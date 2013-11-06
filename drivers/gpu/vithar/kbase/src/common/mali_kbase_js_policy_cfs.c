/*
 *
 * (C) COPYRIGHT 2011 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * 
 */



/*
 * Job Scheduler: Completely Fair Policy Implementation
 */

#include <kbase/src/common/mali_kbase.h>
#include <kbase/src/common/mali_kbase_jm.h>
#include <kbase/src/common/mali_kbase_js.h>
#include <kbase/src/common/mali_kbase_js_policy_cfs.h>

/** Fixed point constants used for runtime weight calculations */
#define WEIGHT_FIXEDPOINT_SHIFT 10
#define WEIGHT_TABLE_SIZE       40
#define WEIGHT_0_NICE           (WEIGHT_TABLE_SIZE/2)
#define WEIGHT_0_VAL            (1 << WEIGHT_FIXEDPOINT_SHIFT)

#define LOOKUP_VARIANT_MASK ((1u<<KBASEP_JS_MAX_NR_CORE_REQ_VARIANTS) - 1u)

/* core_req variants are ordered by least restrictive first, so that our
 * algorithm in cached_variant_idx_init picks the least restrictive variant for
 * each job . Note that coherent_group requirement is added to all CS works as the
 * selection of JS does not depend on the coherency requirement. */
static const base_jd_core_req core_req_variants[] ={

	BASE_JD_REQ_FS | BASE_JD_REQ_CF | BASE_JD_REQ_V | BASE_JD_REQ_PERMON,
	BASE_JD_REQ_CS | BASE_JD_REQ_CF | BASE_JD_REQ_V | BASE_JD_REQ_COHERENT_GROUP | BASE_JD_REQ_PERMON,
	BASE_JD_REQ_CS | BASE_JD_REQ_T  | BASE_JD_REQ_CF | BASE_JD_REQ_V   | BASE_JD_REQ_COHERENT_GROUP | BASE_JD_REQ_PERMON,
	BASE_JD_REQ_CS | BASE_JD_REQ_CF | BASE_JD_REQ_V  | BASE_JD_REQ_NSS | BASE_JD_REQ_COHERENT_GROUP | BASE_JD_REQ_PERMON
};

#define NUM_CORE_REQ_VARIANTS NELEMS(core_req_variants)

static const u32 variants_supported_ss_state[] =
{
	(1u << 0),             /* js[0] uses variant 0 (FS list)*/
	(1u << 2) | (1u << 1), /* js[1] uses variants 1 and 2 (CS and CS+T lists)*/
	(1u << 1)              /* js[2] uses variant 1 (CS list). NOTE: could set to 0 */
};

static const u32 variants_supported_nss_state[] =
{
	(1u << 0),             /* js[0] uses variant 0 (FS list)*/
	(1u << 2) | (1u << 1), /* js[1] uses variants 1 and 2 (CS and CS+T lists)*/
	(1u << 3)              /* js[2] uses variant 3 (NSS list) */
};

/* Defines for easy asserts 'is scheduled'/'is queued'/'is neither queued norscheduled' */
#define KBASEP_JS_CHECKFLAG_QUEUED       (1u << 0) /**< Check the queued state */
#define KBASEP_JS_CHECKFLAG_SCHEDULED    (1u << 1) /**< Check the scheduled state */
#define KBASEP_JS_CHECKFLAG_IS_QUEUED    (1u << 2) /**< Expect queued state to be set */
#define KBASEP_JS_CHECKFLAG_IS_SCHEDULED (1u << 3) /**< Expect scheduled state to be set */

enum
{
	KBASEP_JS_CHECK_NOTQUEUED     = KBASEP_JS_CHECKFLAG_QUEUED,
	KBASEP_JS_CHECK_NOTSCHEDULED  = KBASEP_JS_CHECKFLAG_SCHEDULED,
	KBASEP_JS_CHECK_QUEUED        = KBASEP_JS_CHECKFLAG_QUEUED | KBASEP_JS_CHECKFLAG_IS_QUEUED,
	KBASEP_JS_CHECK_SCHEDULED     = KBASEP_JS_CHECKFLAG_SCHEDULED | KBASEP_JS_CHECKFLAG_IS_SCHEDULED
};

typedef u32 kbasep_js_check;

/*
 * Private Functions
 */

/* Table autogenerated using util built from: kbase/scripts/gen_cfs_weight_of_prio.c */

/* weight = 1.25 */
static const int weight_of_priority[] =
{
	/*  -20 */     11,      14,      18,      23,
	/*  -16 */     29,      36,      45,      56,
	/*  -12 */     70,      88,     110,     137,
	/*   -8 */    171,     214,     268,     335,
	/*   -4 */    419,     524,     655,     819,
	/*    0 */   1024,    1280,    1600,    2000,
	/*    4 */   2500,    3125,    3906,    4883,
	/*    8 */   6104,    7630,    9538,   11923,
	/*   12 */  14904,   18630,   23288,   29110,
	/*   16 */  36388,   45485,   56856,   71070
};

STATIC u64 priority_weight(kbasep_js_policy_cfs_ctx *ctx_info, u32 time_us)
{
	u64 time_delta_us;
	int priority;
	priority = ctx_info->process_priority + ctx_info->bag_priority;

	/* Adjust runtime_us using priority weight if required */
	if(priority != 0)
	{
		int clamped_priority;

		/* Clamp values to min..max weights */
		if(priority > OSK_PROCESS_PRIORITY_MAX)
		{
			clamped_priority = OSK_PROCESS_PRIORITY_MAX;
		}
		else if(priority < OSK_PROCESS_PRIORITY_MIN)
		{
			clamped_priority = OSK_PROCESS_PRIORITY_MIN;
		}
		else
		{
			clamped_priority = priority;
		}

		/* Fixed point multiplication */
		time_delta_us = ((u64)time_us * weight_of_priority[WEIGHT_0_NICE + clamped_priority]);
		/* Remove fraction */
		time_delta_us = time_delta_us >> WEIGHT_FIXEDPOINT_SHIFT;
		/* Make sure the time always increases */
		if(0 == time_delta_us)
		{
			time_delta_us++;
		}
	}
	else
	{
		time_delta_us = time_us;
	}

	return time_delta_us;
}

STATIC void kbasep_js_debug_check( kbasep_js_policy_cfs *policy_info, kbase_context *kctx, kbasep_js_check check_flag )
{
	/* This function uses the ternary operator and non-explicit comparisons,
	 * because it makes for much shorter, easier to read code */

	if ( check_flag & KBASEP_JS_CHECKFLAG_QUEUED )
	{
		mali_bool is_queued;
		mali_bool expect_queued;
		is_queued = ( OSK_DLIST_MEMBER_OF( &policy_info->ctx_queue_head,
		                                   kctx,
		                                   jctx.sched_info.runpool.policy_ctx.cfs.list ) )? MALI_TRUE: MALI_FALSE;

		if(!is_queued)
		{
			is_queued = ( OSK_DLIST_MEMBER_OF( &policy_info->ctx_rt_queue_head,
			                                   kctx,
			                                   jctx.sched_info.runpool.policy_ctx.cfs.list ) )? MALI_TRUE: MALI_FALSE;
		}

		expect_queued = ( check_flag & KBASEP_JS_CHECKFLAG_IS_QUEUED ) ? MALI_TRUE : MALI_FALSE;

		OSK_ASSERT_MSG( expect_queued == is_queued,
		                "Expected context %p to be %s but it was %s\n",
		                kctx,
		                (expect_queued)   ?"queued":"not queued",
		                (is_queued)       ?"queued":"not queued" );

	}

	if ( check_flag & KBASEP_JS_CHECKFLAG_SCHEDULED )
	{
		mali_bool is_scheduled;
		mali_bool expect_scheduled;
		is_scheduled = ( OSK_DLIST_MEMBER_OF( &policy_info->scheduled_ctxs_head,
		                                      kctx,
		                                      jctx.sched_info.runpool.policy_ctx.cfs.list ) )? MALI_TRUE: MALI_FALSE;

		expect_scheduled = ( check_flag & KBASEP_JS_CHECKFLAG_IS_SCHEDULED ) ? MALI_TRUE : MALI_FALSE;
		OSK_ASSERT_MSG( expect_scheduled == is_scheduled,
		                "Expected context %p to be %s but it was %s\n",
		                kctx,
		                (expect_scheduled)?"scheduled":"not scheduled",
		                (is_scheduled)    ?"scheduled":"not scheduled" );

	}

}

STATIC INLINE void set_slot_to_variant_lookup( u32 *bit_array, u32 slot_idx, u32 variants_supported )
{
	u32 overall_bit_idx = slot_idx * KBASEP_JS_MAX_NR_CORE_REQ_VARIANTS;
	u32 word_idx = overall_bit_idx / 32;
	u32 bit_idx = overall_bit_idx % 32;

	OSK_ASSERT( slot_idx < BASE_JM_MAX_NR_SLOTS );
	OSK_ASSERT( (variants_supported & ~LOOKUP_VARIANT_MASK) == 0 );

	bit_array[word_idx] |= variants_supported << bit_idx;
}


STATIC INLINE u32 get_slot_to_variant_lookup( u32 *bit_array, u32 slot_idx )
{
	u32 overall_bit_idx = slot_idx * KBASEP_JS_MAX_NR_CORE_REQ_VARIANTS;
	u32 word_idx = overall_bit_idx / 32;
	u32 bit_idx = overall_bit_idx % 32;

	u32 res;

	OSK_ASSERT( slot_idx < BASE_JM_MAX_NR_SLOTS );

	res = bit_array[word_idx] >> bit_idx;
	res &= LOOKUP_VARIANT_MASK;

	return res;
}

/* Check the core_req_variants: make sure that every job slot is satisifed by
 * one of the variants. This checks that cached_variant_idx_init will produce a
 * valid result for jobs that make maximum use of the job slots. */
#if MALI_DEBUG
STATIC void debug_check_core_req_variants( kbase_device *kbdev, kbasep_js_policy_cfs *policy_info )
{
	kbasep_js_device_data *js_devdata;
	int i;
	int j;

	js_devdata = &kbdev->js_data;

	for ( j = 0 ; j < kbdev->nr_job_slots ; ++j )
	{
		base_jd_core_req job_core_req;
		mali_bool found = MALI_FALSE;

		job_core_req =  js_devdata->js_reqs[j];
		for ( i = 0; i < policy_info->num_core_req_variants ; ++i )
		{
			base_jd_core_req var_core_req;
			var_core_req = policy_info->core_req_variants[i];

			if ( (var_core_req & job_core_req) == job_core_req )
			{
				found = MALI_TRUE;
				break;
			}
		}

		/* Early-out on any failure */
		OSK_ASSERT_MSG( found != MALI_FALSE,
		                "Job slot %d features 0x%x not matched by core_req_variants. "
		                "Rework core_req_variants and vairants_supported_<...>_state[] to match\n",
		                j,
		                job_core_req );
	}
}
#endif

STATIC void build_core_req_variants( kbase_device *kbdev, kbasep_js_policy_cfs *policy_info )
{
	OSK_ASSERT( kbdev != NULL );
	OSK_ASSERT( policy_info != NULL );

	/* Assume a static set of variants */
	OSK_MEMCPY( policy_info->core_req_variants, core_req_variants, sizeof(core_req_variants) );

	policy_info->num_core_req_variants = NUM_CORE_REQ_VARIANTS;

	OSK_DEBUG_CODE( debug_check_core_req_variants( kbdev, policy_info ) );
}


STATIC void build_slot_lookups( kbase_device *kbdev, kbasep_js_policy_cfs *policy_info )
{
	int i;

	/* Given the static set of variants, provide a static set of lookups */
	for ( i = 0; i < kbdev->nr_job_slots; ++i )
	{
		set_slot_to_variant_lookup( policy_info->slot_to_variant_lookup_ss_state,
		                            i,
		                            variants_supported_ss_state[i] );

		set_slot_to_variant_lookup( policy_info->slot_to_variant_lookup_nss_state,
		                            i,
		                            variants_supported_nss_state[i] );
	}

}

STATIC mali_error cached_variant_idx_init( kbasep_js_policy_cfs *policy_info, kbase_jd_atom *atom )
{
	kbasep_js_policy_cfs_job *job_info;
	u32 i;
	base_jd_core_req job_core_req;

	OSK_ASSERT( policy_info != NULL );
	OSK_ASSERT( atom != NULL );

	job_info = &atom->sched_info.cfs;
	job_core_req = atom->atom->core_req;

	/* Pick a core_req variant that matches us. Since they're ordered by least
	 * restrictive first, it picks the least restrictive variant */
	for ( i = 0; i < policy_info->num_core_req_variants ; ++i )
	{
		base_jd_core_req var_core_req;
		var_core_req = policy_info->core_req_variants[i];
		
		if ( (var_core_req & job_core_req) == job_core_req )
		{
			job_info->cached_variant_idx = i;
			return MALI_ERROR_NONE;
		}
	}

	/* Could not find a matching requirement, this should only be caused by an
	 * attempt to attack the driver. */
	return MALI_ERROR_FUNCTION_FAILED;
}

STATIC mali_bool dequeue_job( kbase_device *kbdev,
                              kbase_context *kctx,
                              u32 variants_supported,
                              kbase_jd_atom **katom_ptr)
{
	kbasep_js_device_data *js_devdata;
	kbasep_js_policy_cfs *policy_info;
	kbasep_js_policy_cfs_ctx *ctx_info;

	OSK_ASSERT( kbdev != NULL );
	OSK_ASSERT( katom_ptr != NULL );
	OSK_ASSERT( kctx != NULL );

	js_devdata = &kbdev->js_data;
	policy_info = &js_devdata->policy.cfs;
	ctx_info = &kctx->jctx.sched_info.runpool.policy_ctx.cfs;

	/* Only submit jobs from contexts that are allowed */
	if ( kbasep_js_is_submit_allowed( js_devdata, kctx ) != MALI_FALSE )
	{
		/* Check each variant in turn */
		while ( variants_supported != 0 )
		{
			long variant_idx;
			osk_dlist *job_list;
			variant_idx = osk_find_first_set_bit( variants_supported );
			job_list = &ctx_info->job_list_head[variant_idx];

			if ( OSK_DLIST_IS_EMPTY( job_list ) == MALI_FALSE )
			{
				/* Found a context with a matching job */
				*katom_ptr = OSK_DLIST_POP_FRONT( job_list, kbase_jd_atom, sched_info.cfs.list );

				(*katom_ptr)->sched_info.cfs.ticks = 0;

				/* Put this context at the back of the Run Pool */
				OSK_DLIST_REMOVE( &policy_info->scheduled_ctxs_head,
				                  kctx,
				                  jctx.sched_info.runpool.policy_ctx.cfs.list );
				OSK_DLIST_PUSH_BACK( &policy_info->scheduled_ctxs_head,
				                     kctx,
				                     kbase_context,
				                     jctx.sched_info.runpool.policy_ctx.cfs.list );

				return MALI_TRUE;
			}

			variants_supported &= ~(1u << variant_idx);
		}
		/* All variants checked by here */
	}

	/* The context does not have a  matching job */

	return MALI_FALSE;
}

static void timer_callback(void *data)
{
	kbase_device *kbdev = (kbase_device*)data;
	kbasep_js_device_data *js_devdata;
	kbasep_js_policy_cfs *policy_info;
	int s;
	osk_error osk_err;
	mali_bool reset_needed = MALI_FALSE;

	OSK_ASSERT(kbdev != NULL);

	js_devdata = &kbdev->js_data;
	policy_info = &js_devdata->policy.cfs;

	/* Loop through the slots */
	for(s=0; s<kbdev->nr_job_slots; s++)
	{
		kbase_jm_slot *slot = kbase_job_slot_lock(kbdev, s);

		if (kbasep_jm_nr_jobs_submitted(slot) > 0)
		{
/* The current version of the model doesn't support Soft-Stop */
#if (BASE_HW_ISSUE_5736 == 0) || MALI_BACKEND_KERNEL
			kbase_jd_atom *atom = kbasep_jm_peek_idx_submit_slot(slot, 0);
			u32 ticks = atom->sched_info.cfs.ticks ++;

			if ((atom->atom->core_req & BASE_JD_REQ_NSS) == 0)
			{
				/* Job is Soft-Stoppable */
				if (ticks == js_devdata->soft_stop_ticks)
				{
					/* Job has been scheduled for at least js_devdata->soft_stop_ticks ticks.
					 * Soft stop the slot so we can run other jobs.
					 */
					OSK_PRINT_INFO( OSK_BASE_JM, "Soft-stop" );

#if KBASE_DISABLE_SCHEDULING_SOFT_STOPS == 0
					kbase_job_slot_softstop(kbdev, s, atom);
#endif
				}
				else if (ticks == js_devdata->hard_stop_ticks_ss)
				{
					/* Job has been scheduled for at least js_devdata->hard_stop_ticks_ss ticks.
					 * It should have been soft-stopped by now. Hard stop the slot.
					 */
#if KBASE_DISABLE_SCHEDULING_HARD_STOPS == 0
					OSK_PRINT_WARN(OSK_BASE_JM, "JS: Job Hard-Stopped (took more than %lu ticks at %lu ms/tick)", ticks, js_devdata->scheduling_tick_ns/1000000u );
					kbase_job_slot_hardstop(atom->kctx, s, atom);
#endif
				}
				else if (ticks == js_devdata->gpu_reset_ticks_ss)
				{
					/* Job has been scheduled for at least js_devdata->gpu_reset_ticks_ss ticks.
					 * It should have left the GPU by now. Signal that the GPU needs to be reset.
					 */
					reset_needed = MALI_TRUE;
				}
			}
			else
			{
				/* Job is Non Soft-Stoppable */
				if (ticks == js_devdata->soft_stop_ticks)
				{
					/* Job has been scheduled for at least js_devdata->soft_stop_ticks.
					 * Let's try to soft-stop it even if it's supposed to be NSS.
					 */
					OSK_PRINT_INFO( OSK_BASE_JM, "Soft-stop" );

#if KBASE_DISABLE_SCHEDULING_SOFT_STOPS == 0
					kbase_job_slot_softstop(kbdev, s, atom);
#endif
				}
				else if (ticks == js_devdata->hard_stop_ticks_nss)
				{
					/* Job has been scheduled for at least js_devdata->hard_stop_ticks_nss ticks.
					 * Hard stop the slot.
					 */
#if KBASE_DISABLE_SCHEDULING_HARD_STOPS == 0
					OSK_PRINT_WARN(OSK_BASE_JM, "JS: Job Hard-Stopped (took more than %lu ticks at %lu ms/tick)", ticks, js_devdata->scheduling_tick_ns/1000000u );
					kbase_job_slot_hardstop(atom->kctx, s, atom);
#endif
				}
				else if (ticks == js_devdata->gpu_reset_ticks_nss)
				{
					/* Job has been scheduled for at least js_devdata->gpu_reset_ticks_nss ticks.
					 * It should have left the GPU by now. Signal that the GPU needs to be reset.
					 */
					reset_needed = MALI_TRUE;
				}
			}
#endif /* (BASE_HW_ISSUE_5736 == 0) || MALI_BACKEND_KERNEL */
		}

		kbase_job_slot_unlock(kbdev, s);
	}
	
	if (reset_needed)
	{
		OSK_PRINT_WARN(OSK_BASE_JM, "JS: Job has been on the GPU for too long");
		if (kbase_prepare_to_reset_gpu(kbdev))
		{
			kbase_reset_gpu(kbdev);
		}
	}

	/* the timer is re-issued if there is contexts in the run-pool */
	osk_spinlock_irq_lock(&js_devdata->runpool_irq.lock);

	if (OSK_DLIST_IS_EMPTY(&policy_info->scheduled_ctxs_head) == MALI_FALSE)
	{
		osk_err = osk_timer_start(&policy_info->timer, js_devdata->scheduling_tick_ns/1000000u);
		if (OSK_ERR_NONE != osk_err)
		{
			policy_info->timer_running = MALI_FALSE;
		}
	}
	else
	{
		policy_info->timer_running = MALI_FALSE;
	}

	osk_spinlock_irq_unlock(&js_devdata->runpool_irq.lock);
}

/*
 * Non-private functions
 */

mali_error kbasep_js_policy_init( kbase_device *kbdev )
{
	kbasep_js_device_data *js_devdata;
	kbasep_js_policy_cfs *policy_info;

	OSK_ASSERT( kbdev != NULL );
	js_devdata = &kbdev->js_data;
	policy_info = &js_devdata->policy.cfs;

	OSK_DLIST_INIT( &policy_info->ctx_queue_head );
	OSK_DLIST_INIT( &policy_info->scheduled_ctxs_head );
	OSK_DLIST_INIT( &policy_info->ctx_rt_queue_head );

	if (osk_timer_init(&policy_info->timer) != OSK_ERR_NONE)
	{
		return MALI_ERROR_FUNCTION_FAILED;
	}

	osk_timer_callback_set( &policy_info->timer, timer_callback, kbdev );

	policy_info->timer_running = MALI_FALSE;

	policy_info->head_runtime_us = 0;

	/* Build up the core_req variants */
	build_core_req_variants( kbdev, policy_info );
	/* Build the slot to variant lookups */
	build_slot_lookups(kbdev, policy_info );

	return MALI_ERROR_NONE;
}

void kbasep_js_policy_term( kbasep_js_policy *js_policy )
{
	kbasep_js_policy_cfs     *policy_info;

	OSK_ASSERT( js_policy != NULL );
	policy_info = &js_policy->cfs;

	/* ASSERT that there are no contexts queued */
	OSK_ASSERT( OSK_DLIST_IS_EMPTY( &policy_info->ctx_queue_head ) != MALI_FALSE );
	/* ASSERT that there are no contexts scheduled */
	OSK_ASSERT( OSK_DLIST_IS_EMPTY( &policy_info->scheduled_ctxs_head ) != MALI_FALSE );

	/* ASSERT that there are no contexts queued */
	OSK_ASSERT( OSK_DLIST_IS_EMPTY( &policy_info->ctx_rt_queue_head ) != MALI_FALSE );

	osk_timer_stop(&policy_info->timer);
	osk_timer_term(&policy_info->timer);
}

mali_error kbasep_js_policy_init_ctx( kbase_device *kbdev, kbase_context *kctx )
{
	kbasep_js_device_data *js_devdata;
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbasep_js_policy_cfs     *policy_info;
	osk_process_priority prio;
	int i;

	OSK_ASSERT( kbdev != NULL );
	OSK_ASSERT( kctx != NULL );

	js_devdata = &kbdev->js_data;
	policy_info = &kbdev->js_data.policy.cfs;
	ctx_info = &kctx->jctx.sched_info.runpool.policy_ctx.cfs;

	for ( i = 0 ; i < policy_info->num_core_req_variants ; ++i )
	{
		OSK_DLIST_INIT( &ctx_info->job_list_head[i] );
	}

	/* We don't allow priority increases unless the process is privileged */
	ctx_info->process_privileged = osk_is_privileged();

	osk_get_process_priority(&prio);
	ctx_info->process_rt_policy = prio.is_realtime;
	ctx_info->process_priority = prio.priority;
	ctx_info->bag_total_priority = 0;
	ctx_info->bag_total_nr_atoms = 0;

	/* Initial runtime (relative to least-run context runtime)
	 *
	 * This uses the Policy Queue's most up-to-date head_runtime_us by using the
	 * queue mutex to issue memory barriers - also ensure future updates to
	 * head_runtime_us occur strictly after this context is initialized */
	osk_mutex_lock( &js_devdata->queue_mutex );

	/* No need to hold the the runpool_irq.lock here, because we're initializing
	 * the value, and the context is definitely not being updated in the
	 * runpool at this point. The queue_mutex ensures the memory barrier. */
	ctx_info->runtime_us = policy_info->head_runtime_us +
		priority_weight(ctx_info,
						(u64)js_devdata->cfs_ctx_runtime_init_slices * (u64)(js_devdata->ctx_timeslice_ns/1000u));

	osk_mutex_unlock( &js_devdata->queue_mutex );

	return MALI_ERROR_NONE;
}

void kbasep_js_policy_term_ctx( kbasep_js_policy *js_policy, kbase_context *kctx )
{
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbasep_js_policy_cfs     *policy_info;
	int i;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx != NULL );

	policy_info = &js_policy->cfs;
	ctx_info = &kctx->jctx.sched_info.runpool.policy_ctx.cfs;

	/* ASSERT that no jobs are present */
	for ( i = 0 ; i < policy_info->num_core_req_variants ; ++i )
	{
		OSK_ASSERT( OSK_DLIST_IS_EMPTY( &ctx_info->job_list_head[i] ) != MALI_FALSE );
	}

	/* No work to do */
}


/*
 * Context Management
 */

void kbasep_js_policy_enqueue_ctx( kbasep_js_policy *js_policy, kbase_context *kctx )
{
	kbasep_js_policy_cfs *policy_info;
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbase_context *list_kctx = NULL;
	kbasep_js_device_data *js_devdata;
	osk_dlist *queue_head;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx != NULL );

	policy_info = &js_policy->cfs;
	ctx_info = &kctx->jctx.sched_info.runpool.policy_ctx.cfs;
	js_devdata = CONTAINER_OF( js_policy, kbasep_js_device_data, policy );

	/* ASSERT about scheduled-ness/queued-ness */
	kbasep_js_debug_check( policy_info, kctx, KBASEP_JS_CHECK_NOTQUEUED );

	/* Clamp the runtime to prevent DoS attacks through "stored-up" runtime */
	if (policy_info->head_runtime_us > ctx_info->runtime_us
		+ (u64)js_devdata->cfs_ctx_runtime_min_slices * (u64)(js_devdata->ctx_timeslice_ns/1000u))
	{
		/* No need to hold the the runpool_irq.lock here, because we're essentially
		 * initializing the value, and the context is definitely not being updated in the
		 * runpool at this point. The queue_mutex held by the caller ensures the memory
		 * barrier. */
		ctx_info->runtime_us = policy_info->head_runtime_us
			- (u64)js_devdata->cfs_ctx_runtime_min_slices * (u64)(js_devdata->ctx_timeslice_ns/1000u);
	}

	/* Find the position where the context should be enqueued */
	if(ctx_info->process_rt_policy)
	{
		queue_head = &policy_info->ctx_rt_queue_head;
	}
	else
	{
		queue_head = &policy_info->ctx_queue_head;
	}

	OSK_DLIST_FOREACH( queue_head,
	                   kbase_context,
	                   jctx.sched_info.runpool.policy_ctx.cfs.list,
	                   list_kctx )
	{
		kbasep_js_policy_cfs_ctx *list_ctx_info;
		list_ctx_info  = &list_kctx->jctx.sched_info.runpool.policy_ctx.cfs;

		if ( list_ctx_info->runtime_us > ctx_info->runtime_us )
		{
			break;
		}
	}

	/* Add the context to the queue */
	if (OSK_DLIST_IS_VALID( list_kctx, jctx.sched_info.runpool.policy_ctx.cfs.list ) == MALI_TRUE)
	{
		OSK_DLIST_INSERT_BEFORE( queue_head,
		                         kctx,
		                         list_kctx,
		                         kbase_context,
		                         jctx.sched_info.runpool.policy_ctx.cfs.list );
	}
	else
	{
		OSK_DLIST_PUSH_BACK( queue_head,
		                     kctx,
		                     kbase_context,
		                     jctx.sched_info.runpool.policy_ctx.cfs.list );
	}
}

mali_bool kbasep_js_policy_dequeue_head_ctx( kbasep_js_policy *js_policy, kbase_context **kctx_ptr )
{
	kbasep_js_policy_cfs *policy_info;
	kbase_context *head_ctx;
	osk_dlist *queue_head;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx_ptr != NULL );

	policy_info = &js_policy->cfs;

	/* attempt to dequeue from the 'realttime' queue first */
	if ( OSK_DLIST_IS_EMPTY( &policy_info->ctx_rt_queue_head ) != MALI_FALSE )
	{
		if ( OSK_DLIST_IS_EMPTY( &policy_info->ctx_queue_head ) != MALI_FALSE )
		{
			/* Nothing to dequeue */
			return MALI_FALSE;
		}
		else
		{
			queue_head = &policy_info->ctx_queue_head;
		}
	}
	else
	{
		queue_head = &policy_info->ctx_rt_queue_head;
	}

	/* Contexts are dequeued from the front of the queue */
	*kctx_ptr = OSK_DLIST_POP_FRONT( queue_head,
	                                 kbase_context,
	                                 jctx.sched_info.runpool.policy_ctx.cfs.list );


	/* Update the head runtime */
	head_ctx = OSK_DLIST_FRONT( queue_head,
	                            kbase_context,
	                            jctx.sched_info.runpool.policy_ctx.cfs.list );
	if (OSK_DLIST_IS_VALID( head_ctx, jctx.sched_info.runpool.policy_ctx.cfs.list ) == MALI_TRUE)
	{
		/* No need to hold the the runpool_irq.lock here for reading - the
		 * context is definitely not being updated in the runpool at this
		 * point. The queue_mutex held by the caller ensures the memory barrier. */
		u64 head_runtime = head_ctx->jctx.sched_info.runpool.policy_ctx.cfs.runtime_us;

		if (head_runtime > policy_info->head_runtime_us)
		{
			policy_info->head_runtime_us = head_runtime;
		}
	}

	return MALI_TRUE;
}

mali_bool kbasep_js_policy_try_evict_ctx( kbasep_js_policy *js_policy, kbase_context *kctx )
{
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbasep_js_policy_cfs     *policy_info;
	mali_bool is_present;
	osk_dlist *queue_head;
	osk_dlist *qhead;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx != NULL );

	policy_info = &js_policy->cfs;
	ctx_info = &kctx->jctx.sched_info.runpool.policy_ctx.cfs;

	if(ctx_info->process_rt_policy)
	{
		queue_head = &policy_info->ctx_rt_queue_head;
	}
	else
	{
		queue_head = &policy_info->ctx_queue_head;
	}
	qhead = queue_head;

	is_present = OSK_DLIST_MEMBER_OF( qhead,
	                                  kctx,
	                                  jctx.sched_info.runpool.policy_ctx.cfs.list );

	if ( is_present != MALI_FALSE )
	{
		kbase_context *head_ctx;
		qhead = queue_head;
		/* Remove the context */
		OSK_DLIST_REMOVE( qhead,
		                  kctx,
		                  jctx.sched_info.runpool.policy_ctx.cfs.list );

		qhead = queue_head;
		/* Update the head runtime */
		head_ctx = OSK_DLIST_FRONT( qhead,
		                            kbase_context,
		                            jctx.sched_info.runpool.policy_ctx.cfs.list );
		if (OSK_DLIST_IS_VALID( head_ctx, jctx.sched_info.runpool.policy_ctx.cfs.list ) == MALI_TRUE)
		{
			/* No need to hold the the runpool_irq.lock here for reading - the
			 * context is definitely not being updated in the runpool at this
			 * point. The queue_mutex held by the caller ensures the memory barrier. */
			u64 head_runtime = head_ctx->jctx.sched_info.runpool.policy_ctx.cfs.runtime_us;

			if (head_runtime > policy_info->head_runtime_us)
			{
				policy_info->head_runtime_us = head_runtime;
			}
		}
	}

	return is_present;
}

void kbasep_js_policy_kill_all_ctx_jobs( kbasep_js_policy *js_policy, kbase_context *kctx )
{
	kbasep_js_policy_cfs *policy_info;
	kbasep_js_policy_cfs_ctx *ctx_info;
	u32 i;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx != NULL );

	policy_info = &js_policy->cfs;
	ctx_info = &kctx->jctx.sched_info.runpool.policy_ctx.cfs;

	/* Kill jobs on each variant in turn */
	for ( i = 0; i < policy_info->num_core_req_variants; ++i )
	{
		osk_dlist *job_list;
		job_list = &ctx_info->job_list_head[i];

		/* Call kbase_jd_cancel() on all kbase_jd_atoms in this list, whilst removing them from the list */
		OSK_DLIST_EMPTY_LIST( job_list, kbase_jd_atom, sched_info.cfs.list, kbase_jd_cancel );
	}

}

void kbasep_js_policy_runpool_add_ctx( kbasep_js_policy *js_policy, kbase_context *kctx )
{
	kbasep_js_policy_cfs     *policy_info;
	kbasep_js_device_data    *js_devdata;
	osk_error osk_err;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx != NULL );

	policy_info = &js_policy->cfs;
	js_devdata = CONTAINER_OF( js_policy, kbasep_js_device_data, policy );

	/* ASSERT about scheduled-ness/queued-ness */
	kbasep_js_debug_check( policy_info, kctx, KBASEP_JS_CHECK_NOTSCHEDULED );

	/* All enqueued contexts go to the back of the runpool */
	OSK_DLIST_PUSH_BACK( &policy_info->scheduled_ctxs_head,
	                     kctx,
	                     kbase_context,
	                     jctx.sched_info.runpool.policy_ctx.cfs.list );

	if (policy_info->timer_running == MALI_FALSE)
	{
		osk_err = osk_timer_start(&policy_info->timer, js_devdata->scheduling_tick_ns/1000000u);
		if (OSK_ERR_NONE == osk_err)
		{
			policy_info->timer_running = MALI_TRUE;
		}
	}
}

void kbasep_js_policy_runpool_remove_ctx( kbasep_js_policy *js_policy, kbase_context *kctx )
{
	kbasep_js_policy_cfs     *policy_info;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx != NULL );

	policy_info = &js_policy->cfs;

	/* ASSERT about scheduled-ness/queued-ness */
	kbasep_js_debug_check( policy_info, kctx, KBASEP_JS_CHECK_SCHEDULED );

	/* No searching or significant list maintenance required to remove this context */
	OSK_DLIST_REMOVE( &policy_info->scheduled_ctxs_head,
	                  kctx,
	                  jctx.sched_info.runpool.policy_ctx.cfs.list );
}

mali_bool kbasep_js_policy_should_remove_ctx( kbasep_js_policy *js_policy, kbase_context *kctx )
{
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbasep_js_policy_cfs     *policy_info;
	kbase_context            *head_ctx;
	kbasep_js_device_data    *js_devdata;
	osk_dlist *queue_head;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( kctx != NULL );

	policy_info = &js_policy->cfs;
	ctx_info = &kctx->jctx.sched_info.runpool.policy_ctx.cfs;
	js_devdata = CONTAINER_OF( js_policy, kbasep_js_device_data, policy );

	if(ctx_info->process_rt_policy)
	{
		queue_head = &policy_info->ctx_rt_queue_head;
	}
	else
	{
		queue_head = &policy_info->ctx_queue_head;
	}

	head_ctx = OSK_DLIST_FRONT( queue_head,
	                            kbase_context,
	                            jctx.sched_info.runpool.policy_ctx.cfs.list );
	if (OSK_DLIST_IS_VALID( head_ctx, jctx.sched_info.runpool.policy_ctx.cfs.list ) == MALI_TRUE)
	{
		u64 head_runtime_us = head_ctx->jctx.sched_info.runpool.policy_ctx.cfs.runtime_us;

		if ((head_runtime_us + priority_weight(ctx_info, (u64)(js_devdata->ctx_timeslice_ns/1000u)))
			< ctx_info->runtime_us)
		{
			/* The context is scheduled out if it's not the least-run context anymore.
			 * The "real" head runtime is used instead of the cached runtime so the current
			 * context is not scheduled out when there is less contexts than address spaces.
			 */
			return MALI_TRUE;
		}
	}

	return MALI_FALSE;
}

/*
 * Job Chain Management
 */

mali_error kbasep_js_policy_init_job( kbasep_js_policy *js_policy, kbase_jd_atom *atom )
{
	kbasep_js_policy_cfs *policy_info;
	kbase_context *parent_ctx;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( atom != NULL );
	parent_ctx = atom->kctx;
	OSK_ASSERT( parent_ctx != NULL );

	policy_info = &js_policy->cfs;

	/* Determine the job's index into the job list head, will return error if the
	 * atom is malformed and so is reported. */
	return cached_variant_idx_init( policy_info, atom );
}

void kbasep_js_policy_term_job( kbasep_js_policy *js_policy, kbase_jd_atom *atom )
{
	kbasep_js_policy_cfs_job *job_info;
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbase_context *parent_ctx;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( atom != NULL );
	parent_ctx = atom->kctx;
	OSK_ASSERT( parent_ctx != NULL );

	job_info = &atom->sched_info.cfs;
	ctx_info = &parent_ctx->jctx.sched_info.runpool.policy_ctx.cfs;

	/* This policy is simple enough that nothing is required */

	/* In any case, we'll ASSERT that this job was correctly removed from the relevant lists */
	OSK_ASSERT( OSK_DLIST_MEMBER_OF( &ctx_info->job_list_head[job_info->cached_variant_idx],
	                                 atom,
	                                 sched_info.cfs.list ) == MALI_FALSE );
}

mali_bool kbasep_js_policy_dequeue_job( kbase_device *kbdev,
                                        int job_slot_idx,
                                        kbase_jd_atom **katom_ptr )
{
	kbasep_js_device_data *js_devdata;
	kbasep_js_policy_cfs *policy_info;
	kbase_context *kctx;
	u32 variants_supported;

	OSK_ASSERT( kbdev != NULL );
	OSK_ASSERT( katom_ptr != NULL );
	OSK_ASSERT( job_slot_idx < BASE_JM_MAX_NR_SLOTS );

	js_devdata = &kbdev->js_data;
	policy_info = &js_devdata->policy.cfs;

	/* Get the variants for this slot */
	if ( js_devdata->runpool_irq.nr_nss_ctxs_running == 0 )
	{
		/* SS-state */
		variants_supported = get_slot_to_variant_lookup( policy_info->slot_to_variant_lookup_ss_state, job_slot_idx );
	}
	else
	{
		/* NSS-state */
		variants_supported = get_slot_to_variant_lookup( policy_info->slot_to_variant_lookup_nss_state, job_slot_idx );
	}

	/* First pass through the runpool we consider the realtime priority jobs */
	OSK_DLIST_FOREACH( &policy_info->scheduled_ctxs_head,
	                   kbase_context,
	                   jctx.sched_info.runpool.policy_ctx.cfs.list,
	                   kctx )
	{
		if(kctx->jctx.sched_info.runpool.policy_ctx.cfs.process_rt_policy)
		{
			if(dequeue_job(kbdev, kctx, variants_supported, katom_ptr))
			{
				/* Realtime policy job matched */
				return MALI_TRUE;
			}
		}
	}

	/* Second pass through the runpool we consider the non-realtime priority jobs */
	OSK_DLIST_FOREACH( &policy_info->scheduled_ctxs_head,
	                   kbase_context,
	                   jctx.sched_info.runpool.policy_ctx.cfs.list,
	                   kctx )
	{
		if(kctx->jctx.sched_info.runpool.policy_ctx.cfs.process_rt_policy == MALI_FALSE)
		{
			if(dequeue_job(kbdev, kctx, variants_supported, katom_ptr))
			{
				/* Non-realtime policy job matched */
				return MALI_TRUE;
			}
		}
	}

	/* By this point, no contexts had a matching job */
	return MALI_FALSE;
}

mali_bool kbasep_js_policy_dequeue_job_irq( kbase_device *kbdev,
                                            int job_slot_idx,
                                            kbase_jd_atom **katom_ptr )
{
	/* IRQ and non-IRQ variants of this are the same (though, the IRQ variant could be made faster) */
	return kbasep_js_policy_dequeue_job( kbdev, job_slot_idx, katom_ptr );
}


void kbasep_js_policy_enqueue_job( kbasep_js_policy *js_policy, kbase_jd_atom *katom )
{
	kbasep_js_policy_cfs_job *job_info;
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbase_context *parent_ctx;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( katom != NULL );
	parent_ctx = katom->kctx;
	OSK_ASSERT( parent_ctx != NULL );

	job_info = &katom->sched_info.cfs;
	ctx_info = &parent_ctx->jctx.sched_info.runpool.policy_ctx.cfs;

	OSK_DLIST_PUSH_BACK( &ctx_info->job_list_head[job_info->cached_variant_idx],
	                     katom,
	                     kbase_jd_atom,
	                     sched_info.cfs.list );
}

void kbasep_js_policy_log_job_result( kbasep_js_policy *js_policy, kbase_jd_atom *katom, u32 time_spent_us )
{
	kbasep_js_policy_cfs_ctx *ctx_info;
	kbase_context *parent_ctx;

	OSK_ASSERT( js_policy != NULL );
	OSK_ASSERT( katom != NULL );
	parent_ctx = katom->kctx;
	OSK_ASSERT( parent_ctx != NULL );

	ctx_info = &parent_ctx->jctx.sched_info.runpool.policy_ctx.cfs;

	ctx_info->runtime_us += priority_weight(ctx_info, time_spent_us);
}
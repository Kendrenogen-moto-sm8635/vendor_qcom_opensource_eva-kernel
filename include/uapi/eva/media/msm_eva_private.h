/* SPDX-License-Identifier: GPL-2.0-only WITH Linux-syscall-note */
/*
 * Copyright (c) 2018-2021, The Linux Foundation. All rights reserved.
 */
#ifndef __MSM_EVA_PRIVATE_H__
#define __MSM_EVA_PRIVATE_H__

#include <linux/types.h>

/* Commands type */
#define EVA_KMD_CMD_BASE		0x10000000
#define EVA_KMD_CMD_START		(EVA_KMD_CMD_BASE + 0x1000)

/*
 * userspace clients pass one of the below arguments type
 * in struct eva_kmd_arg (@type field).
 */

/*
 * EVA_KMD_GET_SESSION_INFO - this argument type is used to
 *          get the session information from driver. it passes
 *          struct eva_kmd_session_info {}
 */
#define EVA_KMD_GET_SESSION_INFO	(EVA_KMD_CMD_START + 1)

/*
 * EVA_KMD_REGISTER_BUFFER - this argument type is used to
 *          register the buffer to driver. it passes
 *          struct eva_kmd_buffer {}
 */
#define EVA_KMD_REGISTER_BUFFER		(EVA_KMD_CMD_START + 3)

/*
 * EVA_KMD_REGISTER_BUFFER - this argument type is used to
 *          unregister the buffer to driver. it passes
 *          struct eva_kmd_buffer {}
 */
#define EVA_KMD_UNREGISTER_BUFFER	(EVA_KMD_CMD_START + 4)

#define EVA_KMD_UPDATE_POWER	(EVA_KMD_CMD_START + 17)

#define EVA_KMD_SEND_CMD_PKT	(EVA_KMD_CMD_START + 64)

#define EVA_KMD_RECEIVE_MSG_PKT	 (EVA_KMD_CMD_START + 65)

#define EVA_KMD_SET_SYS_PROPERTY	(EVA_KMD_CMD_START + 66)

#define EVA_KMD_GET_SYS_PROPERTY	(EVA_KMD_CMD_START + 67)

#define EVA_KMD_SESSION_CONTROL		(EVA_KMD_CMD_START + 68)

#define EVA_KMD_SEND_FENCE_CMD_PKT	(EVA_KMD_CMD_START + 69)

#define EVA_KMD_FLUSH_ALL	(EVA_KMD_CMD_START + 70)

#define EVA_KMD_FLUSH_FRAME	(EVA_KMD_CMD_START + 71)

/* flags */
#define EVA_KMD_FLAG_UNSECURE			0x00000000
#define EVA_KMD_FLAG_SECURE			0x00000001

/* buffer type */
#define EVA_KMD_BUFTYPE_INPUT			0x00000001
#define EVA_KMD_BUFTYPE_OUTPUT			0x00000002
#define EVA_KMD_BUFTYPE_INTERNAL_1		0x00000003
#define EVA_KMD_BUFTYPE_INTERNAL_2		0x00000004


/**
 * struct eva_kmd_session_info - session information
 * @session_id:    current session id
 */
struct eva_kmd_session_info {
	__u32 session_id;
	__u32 reserved[10];
};

/**
 * struct eva_kmd_buffer - buffer information to be registered
 * @index:         index of buffer
 * @type:          buffer type
 * @fd:            file descriptor of buffer
 * @size:          allocated size of buffer
 * @offset:        offset in fd from where usable data starts
 * @pixelformat:   fourcc format
 * @flags:         buffer flags
 */
struct eva_kmd_buffer {
	__u32 index;
	__u32 type;
	__u32 fd;
	__u32 size;
	__u32 offset;
	__u32 pixelformat;
	__u32 flags;
	__u32 reserved[5];
};

/**
 * struct eva_kmd_send_cmd - sending generic HFI command
 * @cmd_address_fd:   file descriptor of cmd_address
 * @cmd_size:         allocated size of buffer
 */
struct eva_kmd_send_cmd {
	__u32 cmd_address_fd;
	__u32 cmd_size;
	__u32 reserved[10];
};

/**
 * struct eva_kmd_client_data - store generic client
 *                              data
 * @transactionid:  transaction id
 * @client_data1:   client data to be used during callback
 * @client_data2:   client data to be used during callback
 */
struct eva_kmd_client_data {
	__u32 transactionid;
	__u32 client_data1;
	__u32 client_data2;
};
struct cvp_buf_type {
	__s32 fd;
	__u32 size;
	__u32 offset;
	__u32 flags;
	__u32 reserved1;
	__u32 reserved2;
	__u32 fence_type;
	__u32 input_handle;
	__u32 output_handle;
	__u32 debug_flags;
	__u32 crc;
};

/**
 * Structures and macros for Out-of-Band (OOB) buffer
 * that may accompany HFI packet data
 */

#define EVA_KMD_WNCC_MAX_LAYERS               4
#define EVA_KMD_WNCC_MAX_ADDRESSES            4095
#define EVA_KMD_WNCC_MAX_SRC_BUFS             2400
#define EVA_KMD_WNCC_SRC_BUF_ID_OFFSET        1

/**
 * Macro to get Meta Data Buffer Offset from the HFI Packet
 * The number 15 denotes the position of the first Meta Data Buffer with respect to other fields
 * If the WARPNCC HFI Frame Packet changes, this macro definition also has to be revised
 */

#define EVA_KMD_WNCC_HFI_METADATA_BUFS_OFFSET (15 + sizeof(struct cvp_buf_type) / sizeof(__u32) * 5)

struct eva_kmd_wncc_metadata {
	__u64 loc_x_dec   : 12;
	__u64 loc_x_frac  : 9;
	__u64 loc_y_dec   : 12;
	__u64 loc_y_frac  : 9;
	__u64 iova_lsb    : 22; /* Populated in KMD */
	__u64 iova_msb    : 10; /* Populated in KMD */
	__u64 scale_idx   : 2;
	__s64 aff_coeff_3 : 13;
	__s64 aff_coeff_2 : 13;
	__s64 aff_coeff_1 : 13;
	__s64 aff_coeff_0 : 13;
};

struct eva_kmd_oob_wncc {
	__u32 num_layers;
	struct eva_kmd_wncc_layer {
		__u32 num_addrs;
		struct eva_kmd_wncc_addr {
			__u32 buffer_id;
			__u32 offset;
		} addrs[EVA_KMD_WNCC_MAX_ADDRESSES];
	} layers[EVA_KMD_WNCC_MAX_LAYERS];
};

#define EVA_KMD_OOB_INVALID 0
#define EVA_KMD_OOB_WNCC    1

struct eva_kmd_oob_buf {
	__u32 oob_type;
	union {
		struct eva_kmd_oob_wncc wncc;
	};
};

/**
 * Structures and macros for KMD arg data
 */

#define	MAX_HFI_PKT_SIZE	490

struct eva_kmd_hfi_packet {
	__u32 pkt_data[MAX_HFI_PKT_SIZE];
	struct eva_kmd_oob_buf *oob_buf;
};

#define EVA_KMD_PROP_HFI_VERSION	1
#define EVA_KMD_PROP_SESSION_TYPE	2
#define EVA_KMD_PROP_SESSION_KERNELMASK	3
#define EVA_KMD_PROP_SESSION_PRIORITY	4
#define EVA_KMD_PROP_SESSION_SECURITY	5
#define EVA_KMD_PROP_SESSION_DSPMASK	6
#define EVA_KMD_PROP_SESSION_DUMPOFFSET	7
#define EVA_KMD_PROP_SESSION_DUMPSIZE	8

#define EVA_KMD_PROP_PWR_FDU	0x10
#define EVA_KMD_PROP_PWR_ICA	0x11
#define EVA_KMD_PROP_PWR_OD	0x12
#define EVA_KMD_PROP_PWR_MPU	0x13
#define EVA_KMD_PROP_PWR_FW	0x14
#define EVA_KMD_PROP_PWR_DDR	0x15
#define EVA_KMD_PROP_PWR_SYSCACHE	0x16
#define EVA_KMD_PROP_PWR_FDU_OP	0x17
#define EVA_KMD_PROP_PWR_ICA_OP	0x18
#define EVA_KMD_PROP_PWR_OD_OP	0x19
#define EVA_KMD_PROP_PWR_MPU_OP	0x1A
#define EVA_KMD_PROP_PWR_FW_OP	0x1B
#define EVA_KMD_PROP_PWR_DDR_OP	0x1C
#define EVA_KMD_PROP_PWR_SYSCACHE_OP	0x1D
#define EVA_KMD_PROP_PWR_FPS_FDU	0x1E
#define EVA_KMD_PROP_PWR_FPS_MPU	0x1F
#define EVA_KMD_PROP_PWR_FPS_OD	0x20
#define EVA_KMD_PROP_PWR_FPS_ICA	0x21

#define EVA_KMD_PROP_PWR_VADL 0x22
#define EVA_KMD_PROP_PWR_VADL_OP 0x23
#define EVA_KMD_PROP_PWR_FPS_VADL 0x24

#define EVA_KMD_PROP_PWR_TOF 0x25
#define EVA_KMD_PROP_PWR_TOF_OP 0x26
#define EVA_KMD_PROP_PWR_FPS_TOF 0x27

#define EVA_KMD_PROP_PWR_RGE 0x28
#define EVA_KMD_PROP_PWR_RGE_OP 0x29
#define EVA_KMD_PROP_PWR_FPS_RGE 0x2A

#define EVA_KMD_PROP_PWR_XRA 0x2B
#define EVA_KMD_PROP_PWR_XRA_OP 0x2C
#define EVA_KMD_PROP_PWR_FPS_XRA 0x2D

#define EVA_KMD_PROP_PWR_LSR 0x2E
#define EVA_KMD_PROP_PWR_LSR_OP 0x2F
#define EVA_KMD_PROP_PWR_FPS_LSR 0x30


#define MAX_KMD_PROP_NUM_PER_PACKET		8
#define MAX_KMD_PROP_TYPE	(EVA_KMD_PROP_PWR_FPS_ICA + 1)

struct eva_kmd_sys_property {
	__u32 prop_type;
	__u32 data;
};

struct eva_kmd_sys_properties {
	__u32 prop_num;
	struct eva_kmd_sys_property prop_data[MAX_KMD_PROP_NUM_PER_PACKET];
};

#define SESSION_CREATE	1
#define SESSION_DELETE	2
#define SESSION_START	3
#define SESSION_STOP	4
#define SESSION_INFO	5

struct eva_kmd_session_control {
	__u32 ctrl_type;
	__u32 ctrl_data[8];
};

#define MAX_HFI_FENCE_SIZE	64
#define MAX_HFI_FENCE_OFFSET	MAX_HFI_PKT_SIZE
struct eva_kmd_hfi_fence_packet {
	__u32 pkt_data[MAX_HFI_FENCE_OFFSET];
	__u32 fence_data[MAX_HFI_FENCE_SIZE];
	__u64 frame_id;
};

struct eva_kmd_fence {
#ifdef CVP_CONFIG_SYNX_V2
	__u32 h_synx;
#else
	__s32 h_synx;
	__u32 secure_key;
#endif
};

struct eva_kmd_fence_ctrl {
	__u32 magic;
	__u32 reserved;
	__u64 frame_id;
	__u32 num_fences;
	__u32 output_index;
	struct eva_kmd_fence fences[MAX_HFI_FENCE_SIZE];
};

#define MAX_FENCE_DATA_SIZE	(MAX_HFI_FENCE_SIZE + 6)

struct eva_kmd_hfi_synx_packet {
	__u32 pkt_data[MAX_HFI_PKT_SIZE];
	union {
		__u32 fence_data[MAX_FENCE_DATA_SIZE];
		struct eva_kmd_fence_ctrl fc;
	};
	struct eva_kmd_oob_buf* oob_buf;
};

/**
 * struct eva_kmd_arg
 *
 * @type:          command type
 * @buf_offset:    offset to buffer list in the command
 * @buf_num:       number of buffers in the command
 * @session:       session information
 * @req_power:     power information
 * @regbuf:        buffer to be registered
 * @unregbuf:      buffer to be unregistered
 * @send_cmd:      sending generic HFI command

 * @hfi_pkt:       HFI packet created by user library
 * @sys_properties System properties read or set by user library
 * @hfi_fence_pkt: HFI fence packet created by user library
 */
struct eva_kmd_arg {
	__u32 type;
	__u32 buf_offset;
	__u32 buf_num;
	union eva_data_t {
		struct eva_kmd_session_info session;
		struct eva_kmd_buffer regbuf;
		struct eva_kmd_buffer unregbuf;
		struct eva_kmd_send_cmd send_cmd;
		struct eva_kmd_hfi_packet hfi_pkt;
		struct eva_kmd_sys_properties sys_properties;
		struct eva_kmd_hfi_fence_packet hfi_fence_pkt;
		struct eva_kmd_hfi_synx_packet hfi_synx_pkt;
		struct eva_kmd_session_control session_ctrl;
		__u64 frame_id;
	} data;
};

struct eva_kmd_request_power {
	__u32 deprecated;
};
#endif

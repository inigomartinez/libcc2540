/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __CC2540_ERROR_H__
#define __CC2540_ERROR_H__

#include <stdbool.h>
#include <stdint.h>

#include "cc2540.h"

CC2540_BEGIN_DECLS

typedef enum {
    HCI_SUCCESS,
    HCI_ERROR_FAIL,
    HCI_ERROR_BAD_PARAM,
    HCI_ERROR_BAD_TASK,
    HCI_ERROR_NO_MSG_BUFFER,
    HCI_ERROR_BAD_MSG_POINTER,
    HCI_ERROR_BAD_EVENT_ID,
    HCI_ERROR_BAD_INT_ID,
    HCI_ERROR_NO_TIMER,
    HCI_ERROR_NV_ITEM_UNINIT,
    HCI_ERROR_NV_OPER_FAIL,
    HCI_ERROR_BAD_MEM_SIZE,
    HCI_ERROR_NV_BAD_ITEM_LEN,
    HCI_ERROR_BLE_NOT_READY = 0x10,
    HCI_ERROR_BLE_IN_REQ_MODE,
    HCI_ERROR_BLE_BAD_MODE,
    HCI_ERROR_BLE_MEM_ALLOC,
    HCI_ERROR_BLE_NO_CONN,
    HCI_ERROR_BLE_NO_RESOURCES,
    HCI_ERROR_BLE_PENDING,
    HCI_ERROR_BLE_TIMEOUT,
    HCI_ERROR_BLE_BAD_RANGE,
    HCI_ERROR_BLE_ENCRYPTED_LINK,
    HCI_ERROR_BLE_PROC_COMPLETE,
    HCI_ERROR_BLE_GAP_USER_CANCEL = 0x30,
    HCI_ERROR_BLE_GAP_CONN_NOT_ACCEPT,
    HCI_ERROR_BLE_GAP_BOND_REJECT,
    HCI_ERROR_BLE_BAD_PDU = 0x40,
    HCI_ERROR_BLE_NOT_ENOUGH_AUTHEN,
    HCI_ERROR_BLE_NOT_ENOUGH_ENCRYPT,
    HCI_ERROR_BLE_NOT_ENOUGH_KEY_SIZE,
    HCI_ERROR_BAD_TASK_ID = 0xFF
} __attribute__((packed)) hci_error_t;

static const char *hci_error_str[] = {
    [HCI_SUCCESS]                       = "Success",
    [HCI_ERROR_FAIL]                    = "Failure",
    [HCI_ERROR_BAD_PARAM]               = "Bad parameter",
    [HCI_ERROR_BAD_TASK]                = "Bad task",
    [HCI_ERROR_NO_MSG_BUFFER]           = "No message buffer available",
    [HCI_ERROR_BAD_MSG_POINTER]         = "Bad message pointer",
    [HCI_ERROR_BAD_EVENT_ID]            = "Bad event ID",
    [HCI_ERROR_BAD_INT_ID]              = "Bad interrupt ID",
    [HCI_ERROR_NO_TIMER]                = "No timer available",
    [HCI_ERROR_NV_ITEM_UNINIT]          = "NV item uninitialized",
    [HCI_ERROR_NV_OPER_FAIL]            = "NV operation failed",
    [HCI_ERROR_BAD_MEM_SIZE]            = "Bad memory size",
    [HCI_ERROR_NV_BAD_ITEM_LEN]         = "NV bad item length",
    [HCI_ERROR_BLE_NOT_READY]           = "BLE not ready",
    [HCI_ERROR_BLE_IN_REQ_MODE]         = "BLE in requested mode",
    [HCI_ERROR_BLE_BAD_MODE]            = "BLE bad mode",
    [HCI_ERROR_BLE_MEM_ALLOC]           = "BLE memory allocation error",
    [HCI_ERROR_BLE_NO_CONN]             = "BLE no connection available",
    [HCI_ERROR_BLE_NO_RESOURCES]        = "BLE no resources available",
    [HCI_ERROR_BLE_PENDING]             = "BLE pending",
    [HCI_ERROR_BLE_TIMEOUT]             = "BLE timeout",
    [HCI_ERROR_BLE_BAD_RANGE]           = "BLE bad range",
    [HCI_ERROR_BLE_ENCRYPTED_LINK]      = "BLE encrypted link",
    [HCI_ERROR_BLE_PROC_COMPLETE]       = "BLE procedure complete",
    [HCI_ERROR_BLE_GAP_USER_CANCEL]     = "BLE GAP user cancelled",
    [HCI_ERROR_BLE_GAP_CONN_NOT_ACCEPT] = "BLE GAP connection not acceptable",
    [HCI_ERROR_BLE_GAP_BOND_REJECT]     = "BLE GAP bond rejected",
    [HCI_ERROR_BLE_BAD_PDU]             = "BLE bad PUD",
    [HCI_ERROR_BLE_NOT_ENOUGH_AUTHEN]   = "BLE not enough authentication",
    [HCI_ERROR_BLE_NOT_ENOUGH_ENCRYPT]  = "BLE not enough encryption",
    [HCI_ERROR_BLE_NOT_ENOUGH_KEY_SIZE] = "BLE not enough key size",
    [HCI_ERROR_BAD_TASK_ID]             = "Bad task ID"
};

static inline const char *hci_strerror (hci_error_t code) {
    const char *str = hci_error_str[code];
    if (!str)
        return "Unknown error";
    return str;
}

CC2540_END_DECLS

#endif /* __CC2540_ERROR_H__ */

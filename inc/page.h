#ifndef PAGE_H
#define PAGE_H

#define TCR_CONFIG_REGION_48bit (((64 - 48) << 0) | ((64 - 48) << 16))
#define TCR_CONFIG_4KB ((0b00 << 14) |  (0b10 << 30))
#define TCR_CONFIG_DEFAULT (TCR_CONFIG_REGION_48bit | TCR_CONFIG_4KB)

#define MAIR_DEVICE_nGnRnE 0b00000000
#define MAIR_NORMAL_NOCACHE 0b01000100
#define MAIR_IDX_DEVICE_nGnRnE 0
#define MAIR_IDX_NORMAL_NOCACHE 1

#define PD_TABLE 0b11
#define PD_BLOCK 0b01
#define PD_ACCESS (1 << 10)
#define BOOT_PGD_ATTR PD_TABLE
#define BOOT_PUD_ATTR (PD_ACCESS | (MAIR_IDX_DEVICE_nGnRnE << 2) | PD_BLOCK)
#include <stdint.h>
#define PAGE_MAX 10000
typedef enum {
    FREE,
    ALLOCATED,
} PAGE_STATUS;
struct page_t {
    int id;
    PAGE_STATUS status;
    uint64_t *content;
};

extern struct page_t pages[PAGE_MAX];
extern int page_now;

struct page_t* page_alloc();
void page_free(struct page_t* page);
#endif

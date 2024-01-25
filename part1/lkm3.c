#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/pid.h>
#include <linux/mm.h>
#include <asm/pgtable.h>
#define page_to_phys(page)  (__pfn_to_phys(page_to_pfn(page)))

MODULE_DESCRIPTION("Kernel Module to print the physical address");
MODULE_AUTHOR("Akshay Patidar");
MODULE_LICENSE("GPL");


static int PID=0;
static long virtual_address=0;
module_param(PID,int,S_IRUGO);
module_param(virtual_address,ulong,S_IRUGO);

unsigned long get_physical_address(struct mm_struct* mm,ulong virtualAddress)
{
    pgd_t *pgd; 
    p4d_t *p4d; 
    pud_t *pud; 
    pmd_t *pmd; 
    pte_t *pte; 
    unsigned long physical_address;
    //unsigned long physicalAddress;
    struct page *page;
    pgd = pgd_offset(mm, virtualAddress); 
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        return 0;
    p4d = p4d_offset(pgd, virtualAddress);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
        return 0;
    pud = pud_offset(p4d, virtualAddress);
    if (pud_none(*pud) || pud_bad(*pud))
        return 0;
    pmd = pmd_offset(pud, virtualAddress);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        return 0;
    if (!(pte = pte_offset_map(pmd, virtualAddress)))
        return 0;
    if (!(page = pte_page(*pte)))
        return 0;
    physical_address = page_to_phys(page);
    return physical_address;
}
static int func_init(void)
{
    struct task_struct *task;
    struct pid *pID;
    struct mm_struct *mm;
    unsigned long physical_address;
    pID = find_get_pid(PID);
    task = pid_task(pID,PIDTYPE_PID);
    mm = task->mm;
    physical_address = get_physical_address(mm,virtual_address);
    printk(KERN_INFO"Process ID: %d, Virtual address: %li, Physical address: %ld ",PID,virtual_address,physical_address);
    return 0;
}
static void func_exit(void)
{
    return;
}

module_init(func_init);
module_exit(func_exit);
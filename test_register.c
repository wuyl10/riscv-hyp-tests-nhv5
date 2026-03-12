#include <rvh_test.h>
#include <page_tables.h>



// //当支持double trap时
// TEST_REGISTER(m_double_trap_6);      //要修改.S文件
// TEST_REGISTER(m_double_trap_10);     //要修改.S文件
// TEST_REGISTER(m_double_trap_12);     //要修改.S文件
// TEST_REGISTER(m_double_trap_13);
// TEST_REGISTER(m_double_trap_14);
// TEST_REGISTER(m_double_trap_15);
// TEST_REGISTER(m_double_trap_16);
// TEST_REGISTER(m_double_trap_17);
// TEST_REGISTER(s_double_trap_1);
// TEST_REGISTER(s_double_trap_2);
// TEST_REGISTER(s_double_trap_3);
// TEST_REGISTER(s_double_trap_4);
// TEST_REGISTER(s_double_trap_5);     //要修改.S文件
// TEST_REGISTER(s_double_trap_6);
// TEST_REGISTER(s_double_trap_7);
// TEST_REGISTER(s_double_trap_8);
// TEST_REGISTER(s_double_trap_9);
// TEST_REGISTER(s_double_trap_10);
// TEST_REGISTER(s_double_trap_11);
// TEST_REGISTER(s_double_trap_12);
// TEST_REGISTER(s_double_trap_13);
// TEST_REGISTER(s_double_trap_14);
// TEST_REGISTER(s_double_trap_15);
// TEST_REGISTER(s_double_trap_16);

// // //当支持pointer mask时
// TEST_REGISTER(ssnpm_test_1);
// TEST_REGISTER(ssnpm_test_2);
// TEST_REGISTER(ssnpm_test_3);
// TEST_REGISTER(ssnpm_test_4);
// TEST_REGISTER(ssnpm_test_5); 
// TEST_REGISTER(ssnpm_test_6);
// TEST_REGISTER(ssnpm_test_7);
// TEST_REGISTER(ssnpm_test_8);
// TEST_REGISTER(ssnpm_test_9);
// TEST_REGISTER(ssnpm_test_10);
// TEST_REGISTER(ssnpm_test_11);
// TEST_REGISTER(ssnpm_test_12);
// TEST_REGISTER(ssnpm_test_13);
// TEST_REGISTER(ssnpm_test_14);
// TEST_REGISTER(ssnpm_test_15);
// TEST_REGISTER(ssnpm_test_16);
// TEST_REGISTER(ssnpm_test_17);
// TEST_REGISTER(ssnpm_test_18);
// TEST_REGISTER(ssnpm_test_19);
// TEST_REGISTER(ssnpm_test_20);
// TEST_REGISTER(ssnpm_test_21);
// TEST_REGISTER(ssnpm_test_22);
// TEST_REGISTER(ssnpm_test_23);
// TEST_REGISTER(ssnpm_test_24);
// TEST_REGISTER(ssnpm_test_25);
// TEST_REGISTER(ssnpm_test_26);
// TEST_REGISTER(ssnpm_test_27);
// TEST_REGISTER(ssnpm_test_28);
// TEST_REGISTER(ssnpm_test_29);
// TEST_REGISTER(ssnpm_test_30);
// TEST_REGISTER(ssnpm_test_31);
// TEST_REGISTER(ssnpm_test_32);
// TEST_REGISTER(ssnpm_test_33);
// TEST_REGISTER(ssnpm_test_34);
// TEST_REGISTER(ssnpm_test_35);
// TEST_REGISTER(ssnpm_test_36);
// TEST_REGISTER(ssnpm_test_37);
// TEST_REGISTER(ssnpm_test_38);
// TEST_REGISTER(ssnpm_test_39);
// TEST_REGISTER(ssnpm_test_40);
// TEST_REGISTER(ssnpm_test_41);
// TEST_REGISTER(ssnpm_test_333);
// TEST_REGISTER(smmpm_test_1);
// TEST_REGISTER(smmpm_test_2);
// TEST_REGISTER(smmpm_test_3);
// TEST_REGISTER(smmpm_test_4);
// TEST_REGISTER(smmpm_test_5);
// TEST_REGISTER(smnpm_test_1);
// TEST_REGISTER(smnpm_test_2);
// TEST_REGISTER(smnpm_test_3);
// TEST_REGISTER(smnpm_test_4);
// TEST_REGISTER(smnpm_test_5);



// TEST_REGISTER(stateen_C_test_1); 
// TEST_REGISTER(stateen_C_test_2); 
// TEST_REGISTER(stateen_C_test_3); 
// TEST_REGISTER(stateen_C_test_4);   
// TEST_REGISTER(stateen_C_test_5); 
// TEST_REGISTER(stateen_C_test_6); 
// TEST_REGISTER(stateen_SE0_test_1); 
// TEST_REGISTER(stateen_SE0_test_2); 
// TEST_REGISTER(stateen_SE0_test_3); 
// TEST_REGISTER(stateen_SE0_test_4); 
// TEST_REGISTER(stateen_ENVCFG_test_1); 
// TEST_REGISTER(stateen_ENVCFG_test_2); 
// TEST_REGISTER(stateen_ENVCFG_test_3); 
// TEST_REGISTER(stateen_test_1); 

// TEST_REGISTER(cycle_test_1);
// TEST_REGISTER(cycle_test_2);
// TEST_REGISTER(cycle_test_3);
// TEST_REGISTER(cycle_test_4);
// TEST_REGISTER(cycle_test_5);
// TEST_REGISTER(cycle_test_6);
// TEST_REGISTER(instret_test_1);
// TEST_REGISTER(instret_test_2);
// TEST_REGISTER(instret_test_3);
// TEST_REGISTER(instret_test_4);
// TEST_REGISTER(instret_test_5);
// TEST_REGISTER(instret_test_6);
// TEST_REGISTER(time_test_1);
// TEST_REGISTER(time_test_2);
// TEST_REGISTER(time_test_3);
// TEST_REGISTER(time_test_4);
// TEST_REGISTER(time_test_5);
// TEST_REGISTER(time_test_6);
// TEST_REGISTER(HPM_test_1);
// TEST_REGISTER(HPM_test_2);
// TEST_REGISTER(HPM_test_3);
// TEST_REGISTER(HPM_test_4);
// TEST_REGISTER(HPM_test_5);
// TEST_REGISTER(HPM_test_6);
// TEST_REGISTER(timecmp_test_1);
// TEST_REGISTER(timecmp_test_2);
// TEST_REGISTER(timecmp_test_3);
// TEST_REGISTER(timecmp_test_4);
// TEST_REGISTER(timecmp_test_5);
// TEST_REGISTER(timecmp_test_6);

// TEST_REGISTER(mstatus_csr_tests_1);


    

// TEST_REGISTER(priv_change_8);    
// TEST_REGISTER(priv_change_7);   
// TEST_REGISTER(priv_change_6);    
// TEST_REGISTER(priv_change_5);    
// TEST_REGISTER(priv_change_4);    
// TEST_REGISTER(priv_change_3);   
// TEST_REGISTER(priv_change_2);    
// TEST_REGISTER(priv_change_1); 
// TEST_REGISTER(illegal_except_1);
// TEST_REGISTER(illegal_except_2);
// TEST_REGISTER(illegal_except_3);
// TEST_REGISTER(illegal_except_4);
// TEST_REGISTER(illegal_except_5);
// TEST_REGISTER(illegal_except_6);
// TEST_REGISTER(illegal_except_7);
// TEST_REGISTER(illegal_except_8);
// TEST_REGISTER(illegal_except_9);
// TEST_REGISTER(illegal_except_10);
// TEST_REGISTER(illegal_except_11);
// TEST_REGISTER(illegal_except_12);
// TEST_REGISTER(illegal_except_13);
// TEST_REGISTER(illegal_except_14);
// TEST_REGISTER(illegal_except_15);
// TEST_REGISTER(illegal_except_16);
// TEST_REGISTER(illegal_except_17);

// TEST_REGISTER(wfi_exception_tests_1);
// TEST_REGISTER(wfi_exception_tests_2);
// TEST_REGISTER(wfi_exception_tests_3);
// TEST_REGISTER(wfi_exception_tests_4);
// TEST_REGISTER(wfi_exception_tests_5);
// TEST_REGISTER(wfi_exception_tests_6);
// TEST_REGISTER(wfi_exception_tests_7);
// TEST_REGISTER(wfi_exception_tests_8);
// TEST_REGISTER(wfi_exception_tests_9);
// TEST_REGISTER(wfi_exception_tests_10);
// TEST_REGISTER(wfi_exception_tests_11);
// TEST_REGISTER(wfi_exception_tests_12);
// TEST_REGISTER(wfi_exception_tests_13);
// TEST_REGISTER(wfi_exception_tests_14);
// TEST_REGISTER(wfi_exception_tests_15);
// TEST_REGISTER(wfi_exception_tests_16);


// svpbmt相关
// //l1-cache 用的写回法：cache写命中则不立即写入主存（除非此块被换出）
// TEST_REGISTER(svpbmt_test_1);    //有cache，单独跑rtl
// TEST_REGISTER(svpbmt_test_2);    //有cache，单独跑rtl
// TEST_REGISTER(svpbmt_test_3);
// TEST_REGISTER(svpbmt_test_4);

// TEST_REGISTER(svpbmt_test_5);        
// TEST_REGISTER(svpbmt_test_6);
// TEST_REGISTER(svpbmt_test_7);
// TEST_REGISTER(svpbmt_test_8);
// TEST_REGISTER(svpbmt_test_9);
// TEST_REGISTER(svpbmt_test_10);
// TEST_REGISTER(svpbmt_test_11);       
// TEST_REGISTER(svpbmt_test_12);      
// TEST_REGISTER(svpbmt_test_13);      
// TEST_REGISTER(svpbmt_test_14);
// TEST_REGISTER(svpbmt_test_15);      //涉及cache，单独跑rtl
// TEST_REGISTER(svpbmt_test_16);       //涉及cache，单独跑rtl
// TEST_REGISTER(svpbmt_test_17);        //涉及cache，单独跑rtl
// TEST_REGISTER(svpbmt_test_18);        //涉及cache，单独跑rtl




//页表动态satp.mode相关:Makefile中要修改 c_srcs :=中的page_tables.c和dynamic_page_tables.c根据用例选择其中一个,且增减test.c文件
// TEST_REGISTER(test_sv39_mode);
// TEST_REGISTER(test_sv48_mode);
// TEST_REGISTER(test_sv39_to_sv48_switch);
// TEST_REGISTER(test_sv48_to_sv39_switch);
// TEST_REGISTER(test_multiple_switches_s_mode);
// TEST_REGISTER(test_multiple_switches_u_mode);
// TEST_REGISTER(satp_mode_test);   //涉及tlb，单独跑rtl(代码待设计fence前后取指变化)
// TEST_REGISTER(satp_ppn_test);   //涉及tlb，单独跑rtl
// TEST_REGISTER(satp_ppn_asid_limit_test);     //逻辑太简单，后续需完善


//access fault相关
// TEST_REGISTER(instruction_access_fault_21);  //涉及tlb，单独跑rtl
// TEST_REGISTER(instruction_access_fault_20);  //涉及tlb，单独跑rtl
// TEST_REGISTER(instruction_access_fault_19);  //涉及tlb，单独跑rtl
// TEST_REGISTER(instruction_access_fault_18);
// TEST_REGISTER(instruction_access_fault_17);
// TEST_REGISTER(instruction_access_fault_16);
// TEST_REGISTER(instruction_access_fault_15);
// TEST_REGISTER(instruction_access_fault_14);
// TEST_REGISTER(instruction_access_fault_13);
// TEST_REGISTER(instruction_access_fault_12);
// TEST_REGISTER(instruction_access_fault_11);
// TEST_REGISTER(instruction_access_fault_10);
// TEST_REGISTER(instruction_access_fault_9);
// TEST_REGISTER(instruction_access_fault_8);
// TEST_REGISTER(instruction_access_fault_7);
// TEST_REGISTER(instruction_access_fault_6);
// TEST_REGISTER(instruction_access_fault_5);
// TEST_REGISTER(instruction_access_fault_4);
// TEST_REGISTER(instruction_access_fault_3);
// TEST_REGISTER(instruction_access_fault_2);
// TEST_REGISTER(instruction_access_fault_1);

// TEST_REGISTER(store_access_fault_29);
// TEST_REGISTER(store_access_fault_28);
// TEST_REGISTER(store_access_fault_27);
// TEST_REGISTER(store_access_fault_26);
// TEST_REGISTER(store_access_fault_25);
// TEST_REGISTER(store_access_fault_24);    //涉及tlb，单独跑rtl
// TEST_REGISTER(store_access_fault_23);    //涉及tlb，单独跑rtl
// TEST_REGISTER(store_access_fault_22);    //涉及tlb，单独跑rtl
// TEST_REGISTER(store_access_fault_21);    //涉及tlb，单独跑rtl
// TEST_REGISTER(store_access_fault_20);
// TEST_REGISTER(store_access_fault_19);
// TEST_REGISTER(store_access_fault_18);
// TEST_REGISTER(store_access_fault_17);
// TEST_REGISTER(store_access_fault_16);
// TEST_REGISTER(store_access_fault_15);
// TEST_REGISTER(store_access_fault_14);
// TEST_REGISTER(store_access_fault_13);
// TEST_REGISTER(store_access_fault_12);
// TEST_REGISTER(store_access_fault_11);
// TEST_REGISTER(store_access_fault_10);
// TEST_REGISTER(store_access_fault_9);
// TEST_REGISTER(store_access_fault_8);
// TEST_REGISTER(store_access_fault_7);
// TEST_REGISTER(store_access_fault_6);
// TEST_REGISTER(store_access_fault_5);
// TEST_REGISTER(store_access_fault_4);
// TEST_REGISTER(store_access_fault_3);
// TEST_REGISTER(store_access_fault_2);
// TEST_REGISTER(store_access_fault_1);

// TEST_REGISTER(amo_access_fault_28);
// TEST_REGISTER(amo_access_fault_27);
// TEST_REGISTER(amo_access_fault_26);
// TEST_REGISTER(amo_access_fault_25);
// TEST_REGISTER(amo_access_fault_24);
// TEST_REGISTER(amo_access_fault_23);  //涉及tlb，单独跑rtl
// TEST_REGISTER(amo_access_fault_22);  //涉及tlb，单独跑rtl
// TEST_REGISTER(amo_access_fault_21);  //涉及tlb，单独跑rtl
// TEST_REGISTER(amo_access_fault_20);
// TEST_REGISTER(amo_access_fault_19);
// TEST_REGISTER(amo_access_fault_18);
// TEST_REGISTER(amo_access_fault_17);
// TEST_REGISTER(amo_access_fault_16);
// TEST_REGISTER(amo_access_fault_15);
// TEST_REGISTER(amo_access_fault_14);
// TEST_REGISTER(amo_access_fault_13);
// TEST_REGISTER(amo_access_fault_12);
// TEST_REGISTER(amo_access_fault_11);
// TEST_REGISTER(amo_access_fault_10);
// TEST_REGISTER(amo_access_fault_9);
// TEST_REGISTER(amo_access_fault_8);
// TEST_REGISTER(amo_access_fault_7);
// TEST_REGISTER(amo_access_fault_6);
// TEST_REGISTER(amo_access_fault_5); 
// TEST_REGISTER(amo_access_fault_4);
// TEST_REGISTER(amo_access_fault_3);
// TEST_REGISTER(amo_access_fault_2);
// TEST_REGISTER(amo_access_fault_1);

// TEST_REGISTER(load_access_fault_28);
// TEST_REGISTER(load_access_fault_27);
// TEST_REGISTER(load_access_fault_26);
// TEST_REGISTER(load_access_fault_25);
// TEST_REGISTER(load_access_fault_24);     //涉及tlb，单独跑rtl
// TEST_REGISTER(load_access_fault_23);     //涉及tlb，单独跑rtl
// TEST_REGISTER(load_access_fault_22);     //涉及tlb，单独跑rtl
// TEST_REGISTER(load_access_fault_21);     //涉及tlb，单独跑rtl
// TEST_REGISTER(load_access_fault_20);
// TEST_REGISTER(load_access_fault_19);
// TEST_REGISTER(load_access_fault_18);
// TEST_REGISTER(load_access_fault_17);
// TEST_REGISTER(load_access_fault_16);
// TEST_REGISTER(load_access_fault_15);
// TEST_REGISTER(load_access_fault_14);
// TEST_REGISTER(load_access_fault_13);
// TEST_REGISTER(load_access_fault_12);
// TEST_REGISTER(load_access_fault_11);
// TEST_REGISTER(load_access_fault_10);
// TEST_REGISTER(load_access_fault_9);
// TEST_REGISTER(load_access_fault_8);
// TEST_REGISTER(load_access_fault_7);
// TEST_REGISTER(load_access_fault_6);
// TEST_REGISTER(load_access_fault_5);
// TEST_REGISTER(load_access_fault_4);
// TEST_REGISTER(load_access_fault_3);
// TEST_REGISTER(load_access_fault_2);
// TEST_REGISTER(load_access_fault_1);



//page fault 相关
// TEST_REGISTER(tinst_tests_pf);
// TEST_REGISTER(random_instruction_tests);
// TEST_REGISTER(ebreak_tests_1);
// TEST_REGISTER(ebreak_tests_2);

// TEST_REGISTER(amo_page_fault_1);
// TEST_REGISTER(amo_page_fault_2);
// TEST_REGISTER(amo_page_fault_3);
// TEST_REGISTER(amo_page_fault_4);
// TEST_REGISTER(amo_page_fault_5);
// TEST_REGISTER(amo_page_fault_6);
// TEST_REGISTER(amo_page_fault_7);
// TEST_REGISTER(amo_page_fault_8);
// TEST_REGISTER(amo_page_fault_9);
// TEST_REGISTER(amo_page_fault_10);     //涉及tlb，单独跑rtl
// TEST_REGISTER(amo_page_fault_11);
// TEST_REGISTER(amo_page_fault_12);
// TEST_REGISTER(amo_page_fault_13);
// TEST_REGISTER(amo_page_fault_14);
// TEST_REGISTER(amo_page_fault_15);
// TEST_REGISTER(amo_page_fault_16);
// TEST_REGISTER(amo_page_fault_17);    //sv48时跑，sv39时不跑
// TEST_REGISTER(amo_page_fault_18);    //sv48时跑，sv39时不跑
// TEST_REGISTER(amo_page_fault_19);    //sv48时跑，sv39时不跑
// TEST_REGISTER(store_page_fault_1);
// TEST_REGISTER(store_page_fault_2);
// TEST_REGISTER(store_page_fault_3);
// TEST_REGISTER(store_page_fault_4);
// TEST_REGISTER(store_page_fault_5);
// TEST_REGISTER(store_page_fault_6);
// TEST_REGISTER(store_page_fault_7);
// TEST_REGISTER(store_page_fault_8);
// TEST_REGISTER(store_page_fault_9);
// TEST_REGISTER(store_page_fault_10);
// TEST_REGISTER(store_page_fault_11);
// TEST_REGISTER(store_page_fault_12);
// TEST_REGISTER(store_page_fault_13);
// TEST_REGISTER(store_page_fault_14);
// TEST_REGISTER(store_page_fault_15);
// TEST_REGISTER(store_page_fault_16);
// TEST_REGISTER(store_page_fault_17);    //sv48时跑，sv39时不跑
// TEST_REGISTER(store_page_fault_18);    //sv48时跑，sv39时不跑
// TEST_REGISTER(store_page_fault_19);    //sv48时跑，sv39时不跑
// TEST_REGISTER(load_page_fault_1);
// TEST_REGISTER(load_page_fault_2);
// TEST_REGISTER(load_page_fault_3);
// TEST_REGISTER(load_page_fault_4);
// TEST_REGISTER(load_page_fault_5);
// TEST_REGISTER(load_page_fault_6);        //涉及tlb，单独跑rtl
// TEST_REGISTER(load_page_fault_7);
// TEST_REGISTER(load_page_fault_8);
// TEST_REGISTER(load_page_fault_9);
// TEST_REGISTER(load_page_fault_10);
// TEST_REGISTER(load_page_fault_11);
// TEST_REGISTER(load_page_fault_12);
// TEST_REGISTER(load_page_fault_13);    //sv48时跑，sv39时不跑
// TEST_REGISTER(load_page_fault_14);    //sv48时跑，sv39时不跑
// TEST_REGISTER(load_page_fault_15);    //sv48时跑，sv39时不跑
// TEST_REGISTER(instruction_page_fault_1);
// TEST_REGISTER(instruction_page_fault_2_1);
// TEST_REGISTER(instruction_page_fault_2_2);
// TEST_REGISTER(instruction_page_fault_2_3);
// TEST_REGISTER(instruction_page_fault_3);     //涉及tlb，单独跑rtl
// TEST_REGISTER(instruction_page_fault_4);
// TEST_REGISTER(instruction_page_fault_5);
// TEST_REGISTER(instruction_page_fault_6);
// TEST_REGISTER(instruction_page_fault_7);
// TEST_REGISTER(instruction_page_fault_8);
// TEST_REGISTER(instruction_page_fault_9);
// TEST_REGISTER(instruction_page_fault_10);
// TEST_REGISTER(instruction_page_fault_11);
// TEST_REGISTER(instruction_page_fault_12);
// TEST_REGISTER(instruction_page_fault_13);    //sv48时跑，sv39时不跑
// TEST_REGISTER(instruction_page_fault_14);    //sv48时跑，sv39时不跑
// TEST_REGISTER(instruction_page_fault_15);    //sv48时跑，sv39时不跑
// TEST_REGISTER(instruction_page_fault_16);    //sv48时跑，sv39时不跑
// TEST_REGISTER(instruction_page_fault_17);    //sv48时跑，sv39时不跑
// TEST_REGISTER(instruction_page_fault_18);
// TEST_REGISTER(instruction_page_fault_19);     //涉及tlb，单独跑rtl
// TEST_REGISTER(instruction_page_fault_20);
// TEST_REGISTER(instruction_page_fault_21);
// TEST_REGISTER(instruction_page_fault_22);
// TEST_REGISTER(instruction_page_fault_23);
// TEST_REGISTER(instruction_page_fault_24);
// TEST_REGISTER(instruction_page_fault_25);

// TEST_REGISTER(addr_unaligned_1);  // 触发 LoadUnit s0_no_req_tlb 断言 (MAB writeback vs fast_rep_in 冲突)
// TEST_REGISTER(addr_unaligned_2);
// TEST_REGISTER(addr_unaligned_3);
// TEST_REGISTER(addr_unaligned_4);
// TEST_REGISTER(two_stage_translation_1);   //涉及tlb，单独跑rtl
// TEST_REGISTER(mtvec_test_1);
// TEST_REGISTER(mtvec_test_2);
// TEST_REGISTER(mtvec_test_3);
// TEST_REGISTER(mtvec_test_4);
// TEST_REGISTER(mtvec_test_5);
// TEST_REGISTER(mtvec_test_6);
// TEST_REGISTER(mtvec_test_7);
// TEST_REGISTER(stvec_test_1);
// TEST_REGISTER(stvec_test_2);
// TEST_REGISTER(stvec_test_3);
// TEST_REGISTER(mnret_test_1);      //要修改.S文件
// TEST_REGISTER(m_double_trap_11); 
// TEST_REGISTER(mnret_test_2);
// TEST_REGISTER(rnnmi_handler);
// TEST_REGISTER(script_test);
// TEST_REGISTER(pmp_granularity_test);
// TEST_REGISTER(smrnmi_test_1);    //要修改.S文件
// TEST_REGISTER(smrnmi_test_2);
// TEST_REGISTER(mix_instruction_1);
// TEST_REGISTER(mix_instruction_2);
// TEST_REGISTER(pma_csr_test);

// TEST_REGISTER(menvcfg_cbze_tests_1);
// TEST_REGISTER(menvcfg_cbze_tests_2);
// TEST_REGISTER(senvcfg_cbze_tests_3);
// TEST_REGISTER(menvcfg_cbzfe_tests_1);
// TEST_REGISTER(menvcfg_cbzfe_tests_2);
// TEST_REGISTER(senvcfg_cbzfe_tests_3);
// TEST_REGISTER(menvcfg_cbie_tests_1);
// TEST_REGISTER(menvcfg_cbie_tests_2);
// TEST_REGISTER(senvcfg_cbie_tests_3);

// TEST_REGISTER(pmp_test_1);
// TEST_REGISTER(pmp_test_2);
// TEST_REGISTER(pmp_test_3);
// TEST_REGISTER(pmp_test_4);
// TEST_REGISTER(pmp_test_5);


// TEST_REGISTER(fence_test_1);   //涉及tlb，单独跑rtl
// TEST_REGISTER(fence_test_2);   
// TEST_REGISTER(fence_test_3);   //涉及tlb，单独跑rtl
// TEST_REGISTER(fence_test_4);   //涉及tlb，单独跑rtl

// TEST_REGISTER(caogao);
// TEST_REGISTER(caogao_2);


// TEST_REGISTER(load_access_fault_11);     //要分别弄sv39、sv48跑
// TEST_REGISTER(store_access_fault_11);      //要分别弄sv39、sv48跑    
// TEST_REGISTER(amo_access_fault_11);      //要分别弄sv39、sv48跑

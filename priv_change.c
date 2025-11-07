#include <rvh_test.h>
#include <csrs.h> 
#include <page_tables.h> 




bool priv_change_1(){
    
    TEST_START();


    goto_priv(PRIV_HS);
    hspt_init();
    // hpt_init();
    // vspt_init(); 

    uintptr_t vaddr_f = vs_page_base(VSI_GI);      
    uint64_t value = 0xdeadbeef;

    //V=0，HS模式下发生异常，关闭代理，切换到M态处理异常，处理结束执行mret返回HS态
    TEST_SETUP_EXCEPT();  
    goto_priv(PRIV_M);
    CSRW(medeleg,0);
    CSRW(mideleg,0);
    goto_priv(PRIV_HS);     
    vaddr_f = hs_page_base(VSI_GI);      
    value = 0xdeadbeef;
    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    //mret();
    TEST_ASSERT("hs trigger except that priv change to m mod and mret to hs mode when medeleg/mideleg==0 and hedeleg/hideleg==0",         
        excpt.triggered == true &&
        curr_priv == PRIV_HS &&
        excpt.priv == PRIV_M
    ); 

    TEST_END();
}

bool priv_change_2(){
    
    TEST_START();


    goto_priv(PRIV_HS);
    hspt_init();
    // hpt_init();
    // vspt_init(); 

    uintptr_t vaddr_f = vs_page_base(VSI_GI);      
    uint64_t value = 0xdeadbeef;

    //V=0，HS模式下发生异常，关闭所有代理，切换到M态处理异常
    goto_priv(PRIV_M);
    CSRC(medeleg, 1 << CAUSE_LPF);         
    goto_priv(PRIV_HS);     
    vaddr_f = hs_page_base(VSI_GI);      
    value = 0xdeadbeef;

    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    TEST_ASSERT("hs trigger except that priv change to m mode when medeleg==0",         
        excpt.triggered == true && 
        excpt.cause == CAUSE_LPF &&
        excpt.priv==PRIV_M
    );  
    
    TEST_END();
}

bool priv_change_3(){
    
    TEST_START();


    //V=0，M模式下发生异常，M态处理异常
    goto_priv(PRIV_M);
    reset_state();
    CSRW(medeleg, 0); 

    TEST_SETUP_EXCEPT();       
    ecall_code(); 
    TEST_ASSERT("m trigger except that priv no change when medeleg==0",         
        excpt.triggered == true && 
        excpt.priv==PRIV_M
    ); 
    

    TEST_END();
}

bool priv_change_4(){
    
    TEST_START();


    //V=0，U模式下发生异常，关闭代理，切换到M态处理异常
    goto_priv(PRIV_M);
    CSRC(medeleg,1ULL << 2);         

    goto_priv(PRIV_HU); 
    
    TEST_SETUP_EXCEPT();      
    CSRR(CSR_MSTATUS);
    TEST_ASSERT("hu trigger except that priv change to m mode when medeleg=0",         
        excpt.triggered == true && 
        excpt.priv==PRIV_M &&
        curr_priv == PRIV_HU
    );

    TEST_END();
}





bool priv_change_5(){
    TEST_START();


    //V=0，U模式下发生异常，打开代理medeleg/mideleg，切换到HS态处理异常，异常处理结束执行sret恢复到U态
    goto_priv(PRIV_M);
    reset_state();
    CSRW(medeleg,1ULL << 2);
    CSRW(mideleg,(uint64_t)-1);     
    CSRS(medeleg, (1ULL << 8));       //防止后续u->m 出错

    goto_priv(PRIV_HU);     
    
    TEST_SETUP_EXCEPT();        
    CSRR(CSR_MSTATUS);

    TEST_ASSERT("hu trigger except that priv change to m mod and mret to hu mode when medeleg/mideleg==1",         
        excpt.triggered == true &&
        excpt.priv == PRIV_HS &&
        curr_priv == PRIV_HU
    );

    if(LOG_LEVEL >= LOG_INFO && LOG_LEVEL < LOG_VERBOSE){\
         printf("%s\n" CDFLT, (test_status) ? CGRN "PASSED" : CRED "FAILED");
    }
}



bool priv_change_6(){
    TEST_START();

    //V=0，HS模式下发生异常，打开代理medeleg/mideleg，切换到HS态处理异常

    goto_priv(PRIV_M);     
    CSRW(medeleg,1ULL << 2);
    CSRW(mideleg,(uint64_t)-1);     


    goto_priv(PRIV_HS);
    TEST_SETUP_EXCEPT();        
    CSRR(CSR_MSTATUS);
    TEST_ASSERT("hs trigger except that priv no change when medeleg/mideleg==1 and hedeleg/hideleg==0",         
        excpt.triggered == true &&
        excpt.priv == PRIV_HS
    ); 


    if(LOG_LEVEL >= LOG_INFO && LOG_LEVEL < LOG_VERBOSE){\
         printf("%s\n" CDFLT, (test_status) ? CGRN "PASSED" : CRED "FAILED");
    }

}




bool priv_change_7(){
    TEST_START();

    //V=0，U模式下发生异常，打开代理medeleg/mideleg，关闭代理hedeleg/hideleg，切换到HS态处理异常
    
    goto_priv(PRIV_M);
    reset_state();
    CSRW(medeleg,1ULL << 2);
    CSRW(mideleg,(uint64_t)-1);     
    CSRS(medeleg, (1ULL << 8));       //防止后续u->m 出错

    
    goto_priv(PRIV_HU);     

    TEST_SETUP_EXCEPT();        
    CSRW(medeleg, 0); 

    TEST_ASSERT("hu trigger except that priv change to HS mod and sret to hu mode when medeleg/mideleg==1 and hedeleg/hideleg==0",         
        excpt.triggered == true &&
        excpt.priv == PRIV_HS &&
        curr_priv == PRIV_HU
    );

    TEST_END();

}


bool priv_change_8(){
    TEST_START();

    //ecall to M_mode
    goto_priv(PRIV_HU);
    goto_priv(PRIV_M);
    goto_priv(PRIV_HS);
    goto_priv(PRIV_M);


    // ecall to HS_mode
    goto_priv(PRIV_HU);
    goto_priv(PRIV_HS);

    // random priv change
    for(int i = 0; i < 10; i++){
        int rand1=CSRR(CSR_TIME);  
        int rand2=CSRR(CSR_CYCLE);    
        int num0=rand1+rand2;   
        int idx=num0%3;
        int modes[] = {PRIV_HU, PRIV_HS, PRIV_M};
        goto_priv(modes[idx]);
        printf("Switched to %s mode\n", curr_priv);
    }


    TEST_END();


}

bool priv_change_9(){
    
    TEST_START();


    //U模式下发生异常，打开全部代理，切换到M态处理异常
    goto_priv(PRIV_M);
    CSRW(medeleg,(uint64_t)-1);

    goto_priv(PRIV_HU);

    TEST_SETUP_EXCEPT();
    CSRR(CSR_MCAUSE);
    TEST_ASSERT("hu trigger except that priv change to hs mode when medeleg==1",         
        excpt.triggered == true && 
        excpt.priv==PRIV_HS &&
        curr_priv == PRIV_HU
    );

}

bool priv_change_10(){
    
    TEST_START();


    //M模式下发生异常，打开全部代理，切换到M态处理异常
    goto_priv(PRIV_M);
    CSRW(medeleg,(uint64_t)-1);

    TEST_SETUP_EXCEPT();
    CSRW(CSR_MCONFIGPTR,0xfff);
    TEST_ASSERT("m trigger except that priv change to m mode when medeleg==1",         
        excpt.triggered == true && 
        excpt.priv==PRIV_M &&
        curr_priv == PRIV_M
    );
    
    TEST_END();
}

bool caogao(){
    TEST_START();
    
    //V=0，U模式下发生异常，打开代理medeleg/mideleg，关闭代理hedeleg/hideleg，切换到HS态处理异常
    
    goto_priv(PRIV_M);

    goto_priv(PRIV_HS);
    hspt_init();
    uintptr_t vaddr = vs_page_base(VSRWX_GRWX);
    TEST_SETUP_EXCEPT();        
    lh(vaddr+0xffc);
    excpt_info();


    TEST_END();
}

bool caogao_2() {
    TEST_START();

    //草稿

    // goto_priv(PRIV_HU);
    // vspt_init();
    printf("(uintptr_t)vspt=%lx\n", (uintptr_t)vspt);
    // printf("hpt[0][0]= %lx\n", &hpt[0][0]);
    // printf("hspt[0][0]= %lx\n", &hspt[0][0]);
    // printf("hpt[0][1]= %lx\n", &hpt[0][1]);
    // printf("hpt[0][2]= %lx\n", &hpt[0][2]);
    // printf("hpt[0][0] data=%lx\n", hpt[0][0]);
    printf("MEM_BASE/SUPERPAGE_SIZE(0)=%d\n",MEM_BASE/SUPERPAGE_SIZE(0));
    printf("PAGE_SIZE/sizeof(pte_t)=%d\n",PAGE_SIZE/sizeof(pte_t));
    printf("MEM_SIZE/SUPERPAGE_SIZE(1)/2=%d\n",MEM_SIZE/SUPERPAGE_SIZE(1)/2);

    
    // CSRW(0x7c4, 0xF006F0F00000000);

    TEST_END();
}
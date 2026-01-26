#include <rvh_test.h>
#include <csrs.h> 
#include <page_tables.h>



#define TINST_LOAD(ins) ((ins) & (INS_OPCODE | INS_RD | INS_FUNCT3))
#define TINST_STORE(ins) ((ins) & (INS_OPCODE | INS_FUNCT3 | INS_RS2))
#define TINST_AMO(ins) ((ins) & (~TINST_ADDROFF))
#define TINST_CHECK(CHECK) (excpt.tinst == 0 || excpt.tinst == CHECK(read_instruction(excpt.epc)))
#define TINST_CHECK_COMPRESSED(CHECK) (excpt.tinst == 0 ||\
    excpt.tinst == CHECK(expand_compressed_instruction(read_instruction(excpt.epc)) & ~0b10ULL))



bool mix_instruction_1(){

    TEST_START();

//----------------------------------------------------------------------
    goto_priv(PRIV_M);
    CSRC(CSR_MCOUNTEREN, HCOUNTEREN_TM); 

    goto_priv(PRIV_HS);

    TEST_SETUP_EXCEPT();
    CSRR(CSR_TIME);

    TEST_ASSERT("hs access to time casuses illegal instruction exception when mcounteren.tm=0",
        excpt.triggered == true &&
        excpt.cause == CAUSE_ILI
    );

//----------------------------------------------------------------------

    //mstatus.TW=1时，如果实现了S/HS模式，在U模式下执行WFI指令
    TEST_SETUP_EXCEPT();
    goto_priv(PRIV_M);
    CSRS(mstatus, MSTATUS_TW);

    goto_priv(PRIV_HU);
    wfi();
    
    TEST_ASSERT("HU-mode wfi causes illegal instruction exception when mstatus.tw=1",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_ILI
    ); 
    
//----------------------------------------------------------------------


    goto_priv(PRIV_M);     

    CSRC(CSR_MSTATUS, SSTATUS_SUM);

    goto_priv(PRIV_HS);

    //执行sd指令时，设置了mstatus.SUM=0，在S模式下访问U模式可以访问的页表项
    TEST_SETUP_EXCEPT();
    uintptr_t vaddr = hs_page_base(VSURWX_GURWX);

    
    sd(vaddr, 0xdeadbeef);
    TEST_ASSERT("hs mode sd u mode page when mstatus.sum=0 leads to LPF",
        excpt.triggered == true &&
        excpt.cause == CAUSE_SPF
    );

//----------------------------------------------------------------------
    TEST_SETUP_EXCEPT();
    
    
    goto_priv(PRIV_HS);     
    hspt_init();

    uintptr_t vaddr_f = hs_page_base(VSRWX_GRWX) + 1;      
    uint64_t value = 0xdeadbeef;

    //load byte地址不会发生未对齐
    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    TEST_ASSERT("load byte address is not aligned successful",         
        excpt.triggered == false
    );
//----------------------------------------------------------------------

    TEST_END();

}


bool mix_instruction_2(){

    TEST_START();


//----------------------------------------------------------------------


    goto_priv(PRIV_M);
    CSRC(CSR_MCOUNTEREN, HCOUNTEREN_TM); 

    goto_priv(PRIV_HS);

    TEST_SETUP_EXCEPT();
    CSRR(CSR_TIME);

    TEST_ASSERT("hs access to time casuses illegal instruction exception when mcounteren.tm=0",
        excpt.triggered == true &&
        excpt.cause == CAUSE_ILI
    );

//----------------------------------------------------------------------

    //mstatus.TW=1时，如果实现了S/HS模式，在U模式下执行WFI指令
    TEST_SETUP_EXCEPT();
    goto_priv(PRIV_M);
    CSRS(mstatus, MSTATUS_TW);

    goto_priv(PRIV_HU);
    wfi();
    
    TEST_ASSERT("HU-mode wfi causes illegal instruction exception when mstatus.tw=1",
        excpt.triggered == true &&  
        excpt.cause == CAUSE_ILI
    ); 
    

//----------------------------------------------------------------------



    goto_priv(PRIV_M);     

    CSRC(CSR_MSTATUS, SSTATUS_SUM);

    goto_priv(PRIV_HS);
    hspt_init();

    //执行sd指令时，设置了mstatus.SUM=0，在S模式下访问U模式可以访问的页表项
    TEST_SETUP_EXCEPT();
    uintptr_t vaddr = hs_page_base(VSURWX_GURWX);

    
    sd(vaddr, 0xdeadbeef);
    TEST_ASSERT("hs mode sd u mode page when mstatus.sum=0 leads to LPF",
        excpt.triggered == true &&
        excpt.cause == CAUSE_SPF
    );

//----------------------------------------------------------------------
    TEST_SETUP_EXCEPT();
    
    
    goto_priv(PRIV_HS);     
    uintptr_t vaddr_f = hs_page_base(VSRWX_GRWX) + 1;      
    uint64_t value = 0xdeadbeef;

    //load byte地址不会发生未对齐
    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    TEST_ASSERT("load byte address is not aligned successful",         
        excpt.triggered == false
    );
//----------------------------------------------------------------------

    //m模式下执行ebreak指令
    goto_priv(PRIV_M);
    TEST_SETUP_EXCEPT();
    
    ebreak();

    TEST_ASSERT("m mode ebreak cause to Breakpoint",
        excpt.triggered == true &&
        excpt.cause == CAUSE_BKP
    ); 


//----------------------------------------------------------------------
    goto_priv(PRIV_M);
    CSRC(CSR_MCOUNTEREN, HCOUNTEREN_TM); 

    goto_priv(PRIV_HS);

    TEST_SETUP_EXCEPT();
    CSRR(CSR_TIME);

    TEST_ASSERT("hs access to time casuses illegal instruction exception when mcounteren.tm=0",
        excpt.triggered == true &&
        excpt.cause == CAUSE_ILI
    );


//----------------------------------------------------------------------------------

    goto_priv(PRIV_M);
    
    //pmpcfg.L被设置，当前特权级是M，访问没有执行权限的PMP区域获取指令，pmpcfg.R=0
    CSRW(CSR_PMPCFG0,(uint64_t)0x0);

    CSRC(CSR_PMPCFG0,1ULL << 8 );      //pmp1cfg的R位（清除）
    CSRS(CSR_PMPCFG0,1ULL << 9 );      //pmp1cfg的W位
    CSRS(CSR_PMPCFG0,1ULL << 10 );      //pmp1cfg的X位
    CSRS(CSR_PMPCFG0,1ULL << 11 );      //pmp1cfg的TOR模式

    CSRW(CSR_PMPADDR0, (uintptr_t)0x80000000);
    CSRW(CSR_PMPADDR1, (uintptr_t)0x81000000);
    // sfence();
    // hfence();
    CSRS(CSR_PMPCFG0,1ULL << 15 );       //pmp1cfg的L位 

    TEST_SETUP_EXCEPT();

    lb(0x80000100ULL << 2);    //访问区域内地址


    TEST_ASSERT("m mode lb when pmpcfg.R=0 and pmpcfg.L=1 leads to LAF",        //预期产生load access fault
        excpt.triggered == true &&
        excpt.cause == CAUSE_LAF
    );

//------------------------------------------------------------

    goto_priv(PRIV_M);

    //pmpcfg.L被设置，当前特权级是HS，访问没有执行权限的PMP区域获取指令，pmpcfg.W=0
    
    CSRW(CSR_PMPCFG0,(uint64_t)0x0);
    
    CSRS(CSR_PMPCFG0,1ULL << 0 );      //pmp0cfg的R位
    CSRS(CSR_PMPCFG0,1ULL << 1 );      //pmp0cfg的W位
    CSRS(CSR_PMPCFG0,1ULL << 2 );      //pmp0cfg的X位
    CSRS(CSR_PMPCFG0,1ULL << 3 );      //pmp0cfg的TOR模式


    CSRS(CSR_PMPCFG0,1ULL << 8 );      //pmp1cfg的R位
    CSRC(CSR_PMPCFG0,1ULL << 9 );      //pmp1cfg的W位
    CSRS(CSR_PMPCFG0,1ULL << 10 );      //pmp1cfg的X位
    CSRS(CSR_PMPCFG0,1ULL << 11 );      //pmp1cfg的TOR模式

    CSRW(CSR_PMPADDR0, (uintptr_t)0x80000000);
    CSRW(CSR_PMPADDR1, (uintptr_t)0x81000000);

    CSRS(CSR_PMPCFG0,1ULL << 7 );     //pmp0cfg的L位 
    CSRS(CSR_PMPCFG0,1ULL << 15 );       //pmp1cfg的L位 


    goto_priv(PRIV_HS);
    TEST_SETUP_EXCEPT();    
    
    amoadd_d(0x80000100ULL << 2 , 0xdeadbeef);

    TEST_ASSERT("hs mode amoadd_d when pmpcfg.W=0 and pmpcfg.L=1 leads to SAF",
        excpt.triggered == true &&
        excpt.cause == CAUSE_SAF
    );

//-------------------------------------------------------------

    goto_priv(PRIV_M);

    //pmpcfg.L被设置，当前特权级是HS，访问没有执行权限的PMP区域获取指令，pmpcfg.X=0
    
    CSRW(CSR_PMPCFG0,(uint64_t)0x0);
    
    CSRS(CSR_PMPCFG0,1ULL << 0 );      //pmp0cfg的R位
    CSRS(CSR_PMPCFG0,1ULL << 1 );      //pmp0cfg的W位
    CSRS(CSR_PMPCFG0,1ULL << 2 );      //pmp0cfg的X位
    CSRS(CSR_PMPCFG0,1ULL << 3 );      //pmp0cfg的TOR模式


    CSRS(CSR_PMPCFG0,1ULL << 8 );      //pmp1cfg的R位
    CSRS(CSR_PMPCFG0,1ULL << 9 );      //pmp1cfg的W位
    CSRC(CSR_PMPCFG0,1ULL << 10 );      //pmp1cfg的X位
    CSRS(CSR_PMPCFG0,1ULL << 11 );      //pmp1cfg的TOR模式

    CSRW(CSR_PMPADDR0, (uintptr_t)0x80000000);
    CSRW(CSR_PMPADDR1, (uintptr_t)0x81000000);

    CSRS(CSR_PMPCFG0,1ULL << 7 );     //pmp0cfg的L位 
    CSRC(CSR_PMPCFG0,1ULL << 15 );       //pmp1cfg的L位 

    goto_priv(PRIV_HS);
    TEST_SETUP_EXCEPT();    
    
    TEST_EXEC_EXCEPT(0x80000100ULL << 2);

    TEST_ASSERT("hs mode fetch instruction when pmpcfg.X=0 and pmpcfg.L=1 leads to IAF",
        excpt.triggered == true &&
        excpt.cause == CAUSE_IAF
    );


//-------------------------------------------------------------------------

    TEST_END();

}
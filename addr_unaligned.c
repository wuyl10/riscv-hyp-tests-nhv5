#include <rvh_test.h>
#include <page_tables.h>
#include <csrs.h> 

bool addr_unaligned_1() {

    TEST_START();

    TEST_SETUP_EXCEPT();
    
    goto_priv(PRIV_HS);     
    hspt_init();        

    uintptr_t vaddr_f = hs_page_base(VSRWX_GURWX) + 1;      
    uint64_t value = 0xdeadbeef;
    //load byte地址不会发生未对齐
    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    TEST_ASSERT("load byte address is not aligned successful(mmu open)",         
        excpt.triggered == false
    );

    //load half地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 3;
    value = lh(vaddr_f);
    TEST_ASSERT(" load half address is not aligned result in a lam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM
    );
    
    //load word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    value = lw(vaddr_f);
    TEST_ASSERT("load word address is not aligned result in a lam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM
    );

    //load double word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    value = ld(vaddr_f);
    TEST_ASSERT("load double word address is not aligned result in a lam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM
    );

    //store byte地址不会发生未对齐
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    TEST_SETUP_EXCEPT();        
    sb(vaddr_f,value);
    TEST_ASSERT("store byte address is not aligned successful(mmu open)",         
        excpt.triggered == false
    );

    //store half地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 3;
    TEST_SETUP_EXCEPT();        
    sh(vaddr_f,value);
    TEST_ASSERT("store half address is not aligned result in a sam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM
    );
    
    //store word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    TEST_SETUP_EXCEPT();      
    sw(vaddr_f,value);
    TEST_ASSERT("store word address is not aligned result in a sam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM
    );

    //store double word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    TEST_SETUP_EXCEPT();        
    sd(vaddr_f,value);
    TEST_ASSERT("store double word address is not aligned result in a sam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM
    );

    
    //amo地址未对齐引发sam
    TEST_SETUP_EXCEPT();      
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    amoor_w(vaddr_f, 0xdeadbeef);
    TEST_ASSERT("amo address is not aligned result in a saf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF
    );

    //sc.w地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;   
    sc_w(vaddr_f,value);
    TEST_ASSERT("sc.w address is not aligned result in a saf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF
    );

    //sc.d地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;   
    sc_d(vaddr_f,value);
    TEST_ASSERT("sc.d address is not aligned result in a saf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF
    );

    //lr.w地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5; 
    lr_w(vaddr_f);
    TEST_ASSERT("lr.w address is not aligned result in a laf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF
    );
    
    //lr.d地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7; 
    lr_d(vaddr_f);
    TEST_ASSERT("lr.d address is not aligned result in a laf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF
    );


    TEST_END();

}

bool addr_unaligned_2() {

    TEST_START();

    TEST_SETUP_EXCEPT();
    //开启异常代理
    CSRW(CSR_MEDELEG,(uint64_t)-1);
    
    hspt_init();        
    goto_priv(PRIV_HS);     
    uintptr_t vaddr_f = hs_page_base(VSRWX_GURWX) + 1;      
    uint64_t value = 0xdeadbeef;
    //load byte地址不会发生未对齐
    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    TEST_ASSERT("load byte address is not aligned successful(mmu open)",         
        excpt.triggered == false
    );

    //load half地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 3;
    value = lh(vaddr_f);
    TEST_ASSERT(" load half address is not aligned result in a lam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM&&
        excpt.priv == PRIV_HS
    );
    
    //load word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    value = lw(vaddr_f);
    TEST_ASSERT("load word address is not aligned result in a lam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM&&
        excpt.priv == PRIV_HS
    );

    //load double word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    value = ld(vaddr_f);
    TEST_ASSERT("load double word address is not aligned result in a lam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM&&
        excpt.priv == PRIV_HS
    );

    //store byte地址不会发生未对齐
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    TEST_SETUP_EXCEPT();        
    sb(vaddr_f,value);
    TEST_ASSERT("store byte address is not aligned successful(mmu open)",         
        excpt.triggered == false
    );

    //store half地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 3;
    TEST_SETUP_EXCEPT();        
    sh(vaddr_f,value);
    TEST_ASSERT("store half address is not aligned result in a sam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM&&
        excpt.priv == PRIV_HS
    );
    
    //store word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    TEST_SETUP_EXCEPT();      
    sw(vaddr_f,value);
    TEST_ASSERT("store word address is not aligned result in a sam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM&&
        excpt.priv == PRIV_HS
    );

    //store double word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    TEST_SETUP_EXCEPT();        
    sd(vaddr_f,value);
    TEST_ASSERT("store double word address is not aligned result in a sam(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM&&
        excpt.priv == PRIV_HS
    );

    
    //amo地址未对齐引发sam
    TEST_SETUP_EXCEPT();      
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    amoor_w(vaddr_f, 0xdeadbeef);
    TEST_ASSERT("amo address is not aligned result in a saf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF&&
        excpt.priv == PRIV_HS
    );

    //sc.w地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;   
    sc_w(vaddr_f,value);
    TEST_ASSERT("sc.w address is not aligned result in a saf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF&&
        excpt.priv == PRIV_HS
    );

    //sc.d地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;   
    sc_d(vaddr_f,value);
    TEST_ASSERT("sc.d address is not aligned result in a saf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF&&
        excpt.priv == PRIV_HS
    );

    //lr.w地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5; 
    lr_w(vaddr_f);
    TEST_ASSERT("lr.w address is not aligned result in a laf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF&&
        excpt.priv == PRIV_HS
    );
    
    //lr.d地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7; 
    lr_d(vaddr_f);
    TEST_ASSERT("lr.d address is not aligned result in a laf(mmu open)",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF&&
        excpt.priv == PRIV_HS
    );


}

bool addr_unaligned_3() {

    TEST_START();

    TEST_SETUP_EXCEPT();
    
    goto_priv(PRIV_M);     

    uintptr_t vaddr_f = 0x80000000 + 1;      
    uint64_t value = 0xdeadbeef;
    //load byte地址不会发生未对齐
    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    TEST_ASSERT("m-mode load byte address is not aligned successful",         
        excpt.triggered == false
    );

    //load half地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = 0x80000000 + 3;
    value = lh(vaddr_f);
    TEST_ASSERT("m-mode load half address is not aligned result in a lam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM
    );
    
    //load word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    value = lw(vaddr_f);
    TEST_ASSERT("m-mode load word address is not aligned result in a lam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM
    );

    //load double word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    value = ld(vaddr_f);
    TEST_ASSERT("m-mode load double word address is not aligned result in a lam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM
    );

    //store byte地址不会发生未对齐
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    TEST_SETUP_EXCEPT();        
    sb(vaddr_f,value);
    TEST_ASSERT("m-mode store byte address is not aligned successful",         
        excpt.triggered == false
    );

    //store half地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 3;
    TEST_SETUP_EXCEPT();        
    sh(vaddr_f,value);
    TEST_ASSERT("m-mode store half address is not aligned result in a sam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM
    );
    
    //store word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    TEST_SETUP_EXCEPT();      
    sw(vaddr_f,value);
    TEST_ASSERT("m-mode store word address is not aligned result in a sam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM
    );

    //store double word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    TEST_SETUP_EXCEPT();        
    sd(vaddr_f,value);
    TEST_ASSERT("m-mode store double word address is not aligned result in a sam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM
    );

    
    //amo地址未对齐引发sam
    TEST_SETUP_EXCEPT();      
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    amoor_w(vaddr_f, 0xdeadbeef);
    TEST_ASSERT("m-mode amo address is not aligned result in a saf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF
    );

    //sc.w地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;   
    sc_w(vaddr_f,value);
    TEST_ASSERT("m-mode sc.w address is not aligned result in a saf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF
    );

    //sc.d地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;   
    sc_d(vaddr_f,value);
    TEST_ASSERT("m-mode sc.d address is not aligned result in a saf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF
    );

    //lr.w地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5; 
    lr_w(vaddr_f);
    TEST_ASSERT("m-mode lr.w address is not aligned result in a laf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF
    );
    
    //lr.d地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7; 
    lr_d(vaddr_f);
    TEST_ASSERT("m-mode lr.d address is not aligned result in a laf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF
    );


    TEST_END();

}

bool addr_unaligned_4() {

    TEST_START();

    TEST_SETUP_EXCEPT();
    //开启异常代理
    CSRW(CSR_MEDELEG,(uint64_t)-1);
    
    goto_priv(PRIV_M);     
    uintptr_t vaddr_f = hs_page_base(VSRWX_GURWX) + 1;      
    uint64_t value = 0xdeadbeef;
    //load byte地址不会发生未对齐
    TEST_SETUP_EXCEPT();        
    value = lb(vaddr_f);
    TEST_ASSERT("m-mode load byte address is not aligned successful",         
        excpt.triggered == false
    );

    //load half地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 3;
    value = lh(vaddr_f);
    TEST_ASSERT("m-mode load half address is not aligned result in a lam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM&&
        excpt.priv == PRIV_M
    );
    
    //load word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    value = lw(vaddr_f);
    TEST_ASSERT("m-mode load word address is not aligned result in a lam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM&&
        excpt.priv == PRIV_M
    );

    //load double word地址未对齐引发lam
    TEST_SETUP_EXCEPT();        
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    value = ld(vaddr_f);
    TEST_ASSERT("m-mode load double word address is not aligned result in a lam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAM&&
        excpt.priv == PRIV_M
    );

    //store byte地址不会发生未对齐
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    TEST_SETUP_EXCEPT();        
    sb(vaddr_f,value);
    TEST_ASSERT("m-mode store byte address is not aligned successful",         
        excpt.triggered == false
    );

    //store half地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 3;
    TEST_SETUP_EXCEPT();        
    sh(vaddr_f,value);
    TEST_ASSERT("m-mode store half address is not aligned result in a sam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM&&
        excpt.priv == PRIV_M
    );
    
    //store word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;
    TEST_SETUP_EXCEPT();      
    sw(vaddr_f,value);
    TEST_ASSERT("m-mode store word address is not aligned result in a sam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM&&
        excpt.priv == PRIV_M
    );

    //store double word地址未对齐引发sam
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;
    TEST_SETUP_EXCEPT();        
    sd(vaddr_f,value);
    TEST_ASSERT("m-mode store double word address is not aligned result in a sam",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAM&&
        excpt.priv == PRIV_M
    );

    
    //amo地址未对齐引发sam
    TEST_SETUP_EXCEPT();      
    vaddr_f = hs_page_base(VSRWX_GURWX) + 1;
    amoor_w(vaddr_f, 0xdeadbeef);
    TEST_ASSERT("m-mode amo address is not aligned result in a saf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF&&
        excpt.priv == PRIV_M
    );

    //sc.w地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5;   
    sc_w(vaddr_f,value);
    TEST_ASSERT("m-mode sc.w address is not aligned result in a saf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF&&
        excpt.priv == PRIV_M
    );

    //sc.d地址未对齐引发sam
    TEST_SETUP_EXCEPT();
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7;   
    sc_d(vaddr_f,value);
    TEST_ASSERT("m-mode sc.d address is not aligned result in a saf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_SAF&&
        excpt.priv == PRIV_M
    );

    //lr.w地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 5; 
    lr_w(vaddr_f);
    TEST_ASSERT("m-mode lr.w address is not aligned result in a laf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF&&
        excpt.priv == PRIV_M
    );
    
    //lr.d地址未对齐引发lam
    TEST_SETUP_EXCEPT();       
    vaddr_f = hs_page_base(VSRWX_GURWX) + 7; 
    lr_d(vaddr_f);
    TEST_ASSERT("m-mode lr.d address is not aligned result in a laf",         
        excpt.triggered == true&&
        excpt.cause == CAUSE_LAF&&
        excpt.priv == PRIV_M
    );


}

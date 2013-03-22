// SoCDAM Course Nominal Processor:   (C) DJ Greaves, 2009.  
// $Id: $
// 
// 
//
// This file contains a SystemC testbench with two CPUs, each with local resources connected in RTL style. 
// This is needed for toyclass 5.

#define uint32 unsigned int
// Perhaps instead defined as sc_uint<32> ?


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "systemc.h"
#include "../djip/nominalproc_tlm.h"
#include "djip/clock100.h"
#include "djip/addr_decode.h"
#include "djip/tlmram32.h"
//#include "djip/tlm_busmux.h"
#include "../djip/tlm_busmux_init.h"
#include "../djip/tlmram32port8.h"


ASM test_program[] = 
  {
    { "start", ORG,  0, 0, "Reset to here" },
    { "io_base", EQU,  0x20000, 0, "Output device" },
    { "",      ST,  CID, L(io_base), "" },
    { "",      HALT, 0, 0, "" },
    { "",      END,  0, 0, "" } // Terminate end of program

  }
;
 

//
// A processor nominal unit with local instruction RAM.
//
SC_MODULE(nominalunit)
{

  sc_in <bool> rst, clk, irq;
//  sc_port <memport_if> port1;
//  tlm_utils::simple_initiator_socket<nominalunit> port1; 
  assembler *A;
  uint32 MAX, *code;
  tlm::tlm_initiator_socket<32> port1;

  nominalproc_tlm *nominal_0;
  tlm_busmux *busmux_0;
  addr_decode *addr_decode_0;
  tlmram32 *code_memory_0;


  sc_signal <bool> opreq, opreq0, opreq1;
  sc_signal <uint32> addr;
  SC_CTOR(nominalunit)
   {
    MAX=128;
    uint32 *code = (uint32 *) malloc(MAX * sizeof(uint32));
    A = new assembler(code, MAX); 
    A->assemble(name(), test_program);
    
    nominal_0=new nominalproc_tlm("nominal_0");
    busmux_0 =new tlm_busmux("busmux_0");
    addr_decode_0=new addr_decode("addr_decode_0");
    code_memory_0=new tlmram32("code_memory_0");
//    port1=new tlmram32port8("port1"); 

    addr_decode_0->threshold = 0x10000;
    addr_decode_0->y0(opreq0);
    addr_decode_0->y1(opreq1);
    addr_decode_0->g(opreq);
    addr_decode_0->addr(addr);
    


    code_memory_0->contents(code, MAX);
//    
//    code_memory_0->opack(opack0);
//    code_memory_0->rdata(rdata0);
//    code_memory_0->opreq(opreq0);
//    code_memory_0->rst(rst);
//    code_memory_0->clk(clk);
//    code_memory_0->wdata(wdata);
//    code_memory_0->addr(addr);
//    code_memory_0->rwbar(rwbar);
//    port1->contents(0,MAX);


    busmux_0->threshold = 0x10000;
    busmux_0->port0(*code_memory_0);
    busmux_0->port1.bind(port1);	

    nominal_0->port0(*busmux_0);
    nominal_0->rst(rst);
    nominal_0->irq(irq);   

  }

};



int sc_main(int argc,char *argv[])
{
  sc_signal<bool> clk, rst;
  clock100 u_clkgen("u_clkgen");
  u_clkgen.clk(clk);
/*  MAX=128;
  uint32 *code = (uint32 *) malloc(MAX * sizeof(uint32));
  A = new assembler(code, MAX); 
  A->assemble(name(), test_program);
*/
  tlmram32port8 *datamemory=new tlmram32port8("datamemory");
  datamemory->contents(0,128);
//  port0.register_b_transport(datamemory,&datamemory::b_transport);
   // First processor unit (A): 
  sc_signal <bool>  irq_0;
//  sc_signal <memport_if> port1_0;

  nominalunit *nominalunit_0 = new nominalunit("nomianl_0");
  nominalunit_0->clk(clk);
  nominalunit_0->rst(rst);
  nominalunit_0->irq(irq_0);
  nominalunit_0->port1.bind(datamemory->port0);
  
//  nominalunit_0->port1(port1_0);

  sc_signal <bool>  irq_1;
//  sc_signal <memport_if> port1_1;

  nominalunit *nominalunit_1 = new nominalunit("nominal_1");
  nominalunit_1->clk(clk);
  nominalunit_1->rst(rst);
  nominalunit_1->irq(irq_1);
  nominalunit_1->port1.bind(datamemory->port1);
//  nominalunit_1->port1(port1_1);

  sc_signal <bool>  irq_2;
//  sc_signal <memport_if> port1_2;

  nominalunit *nominalunit_2 = new nominalunit("nominal_2");
  nominalunit_2->clk(clk);
  nominalunit_2->rst(rst);
  nominalunit_2->irq(irq_2);
  nominalunit_2->port1.bind(datamemory->port2);
 // nominalunit_2->port1(port1_2);

  sc_signal <bool>  irq_3;
//  sc_signal <memport_if> port1_3;

  nominalunit *nominalunit_3 = new nominalunit("nominal_3");
  nominalunit_3->clk(clk);
  nominalunit_3->rst(rst);
  nominalunit_3->irq(irq_3);
  nominalunit_3->port1.bind(datamemory->port3);
//  nominalunit_3->port1(port1_3);
 
  sc_signal <bool>  irq_4;
//  sc_signal <memport_if> port1_4;

  nominalunit *nominalunit_4 = new nominalunit("nominal_4");
  nominalunit_4->clk(clk);
  nominalunit_4->rst(rst);
  nominalunit_4->irq(irq_4);
  nominalunit_4->port1.bind(datamemory->port4);
//  nominalunit_4->port1(port1_4);
 
  sc_signal <bool>   irq_5;
//  sc_signal <memport_if> port1_5;
  
  nominalunit *nominalunit_5 = new nominalunit("nominal_5");
  nominalunit_5->clk(clk);
  nominalunit_5->rst(rst);
  nominalunit_5->irq(irq_5);
  nominalunit_5->port1.bind(datamemory->port5);
//  nominalunit_5->port1(port1_5);

  sc_signal <bool>  irq_6;
//  sc_signal <memport_if> port1_6;
 
  nominalunit *nominalunit_6 = new nominalunit("nominal_6");
  nominalunit_6->clk(clk);
  nominalunit_6->rst(rst);
  nominalunit_6->irq(irq_6);
  nominalunit_6->port1.bind(datamemory->port6);
//  nominalunit_6->port1(port1_6);

#if 1
  // Second processor unit (B): 
  sc_signal <bool>  irq_7;
  nominalunit *nominalunit_7 = new nominalunit("nominal_7");
  nominalunit_7->clk(clk);
  nominalunit_7->rst(rst);
  nominalunit_7->irq(irq_7);
  nominalunit_7->port1.bind(datamemory->port7);
//  nominalunit_7->port1(port1_7);
#endif



  irq_0  = 0;
  irq_1  = 0;
  irq_2  = 0;
  irq_3  = 0;
  irq_4  = 0;
  irq_5  = 0;
  irq_6  = 0;
  irq_7  = 0;

  rst = 1; 
  sc_start(100, SC_NS);
  rst = 0;
  printf("End of reset\n");
  sc_start(5, SC_US);
  cout << ("Interrupt to B\n") << sc_time_stamp() << "\n";
  sc_start(10, SC_US);

  cout << "Dual CPU SystemC nominal proc finished at " << sc_time_stamp() << "\n";
  return 0;

}

// eof

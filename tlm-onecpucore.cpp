// SoCDAM Course Nominal Processor:   (C) DJ Greaves, 2009.  
// $Id: $
// 
//  
//
// This file contains a SystemC testbench for a single TLM-style  CPU and two RAM banks and a bit more at the moment....


#define uint32 unsigned int
// Perhaps instead defined as sc_uint<32> ?


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "systemc.h"
#include "djip/nominalproc_tlm.h"
#include "djip/clock100.h"
#include "djip/tlmram32.h"
#include "djip/tlm_busmux.h"
#include "djip/addr_decode.h"






ASM test_program[] = 
  {
    { "start", ORG,  0, 0, "Reset to here" },
    { "io_base", EQU,  0x20000, 0, "Output device" },
    { 0,      JMP,  L(enter), 0, "" },
    { "unity", DEFW,  1, 0, "" },


    { "enter", LOAD,  R1, L(unity), "" },
    { "loop",  ADD,  R1, R1, "Double until wraps to zero" },
    { "",      STORE,  R1, L(io_base), "" },
    { "",      BNZ,  R1, L(loop), "" },
    { "",      HALT, 0, 0, "" },
    { "",      END,  0, 0, "" } // Terminate end of program

  }
;

 


//
//
//

int sc_main(int argc,char *argv[])
{
  assembler *A;
  uint32 MAX = 128;
  uint32 *code = (uint32 *) malloc(MAX * sizeof(uint32));
  A = new assembler(code, MAX); 
  A->assemble("test_program", test_program);


  sc_trace_file *tf = sc_create_vcd_trace_file("trace");
  tf->set_time_unit(1, SC_NS);


  sc_signal <bool> rst, clk, irq;
  sc_signal <uint32> addr;
  sc_signal <bool> opreq, opreq1, opreq0;

/*
#define VCD(X)  sc_trace(tf, X, #X);
  VCD(clk);  VCD(rst);
  VCD(irq);  VCD(addr); VCD(rwbar);
  VCD(wdata);   VCD(rdata);
  VCD(opreq1);  VCD(opreq0);
  VCD(opack1);  VCD(opack0);
  VCD(opack);  VCD(opack);

*/



  clock100 u_clkgen("u_clkgen");
  u_clkgen.clk(clk);




  addr_decode addr_decode_0("addr_decode_0");
  addr_decode_0.threshold = 0x10000;
  addr_decode_0.y0(opreq0);
  addr_decode_0.y1(opreq1);
  addr_decode_0.g(opreq);
  addr_decode_0.addr(addr);


  // TLM style components now follow

  tlmram32 code_memory_0("code_memory_0");
  code_memory_0.contents(code, MAX);
 
  tlmram32 data_memory_0("data_memory_0");
  data_memory_0.contents(0, MAX);


  tlm_busmux tlm_busmux_0("tlm_busmux_0");
  tlm_busmux_0.threshold = 0x10000;
  tlm_busmux_0.port0(code_memory_0);
  tlm_busmux_0.port1(data_memory_0);

  nominalproc_tlm nominal_0("nominal_0");
  nominal_0.rst(rst);
  nominal_0.irq(irq);
  nominal_0.port0(tlm_busmux_0);
 




  // Testbench now follows

  rst = 1; irq = 0; 
  sc_start(100, SC_NS);
  rst = 0;
  printf("End of reset\n");
  sc_start(10, SC_US);

  cout << "Single CPU SystemC nominal proc finished at " << sc_time_stamp() << "\n";
  sc_close_vcd_trace_file(tf);
  return 0;
}


// eof

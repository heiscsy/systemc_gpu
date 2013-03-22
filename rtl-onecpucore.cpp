// SoCDAM Course Nominal Processor:   (C) DJ Greaves, 2009.  
// $Id: $
// 
// 
//
// This file contains a SystemC testbench for a single CPU and two RAM banks.


#define uint32 unsigned int
// Perhaps instead defined as sc_uint<32> ?


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "systemc.h"
#include "djip/nominalproc_rtl.h"
#include "djip/clock100.h"
#include "djip/ram32.h"
#include "djip/busmux.h"
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
 


int main()
{
  assembler *A;
  uint32 MAX = 128;
  uint32 *code = (uint32 *) malloc(MAX * sizeof(uint32));
  A = new assembler(code, MAX); 
  A->assemble("test_program", test_program);


  sc_trace_file *tf = sc_create_vcd_trace_file("trace");
  tf->set_time_unit(1, SC_NS);


  sc_signal <bool> rst, clk, irq;
  sc_signal <uint32> addr, wdata;
  sc_signal <uint32> rdata, rdata1, rdata0;
  sc_signal <bool> opreq, opreq1, opreq0, rwbar;
  sc_signal <bool> opack, opack1, opack0;


#define VCD(X)  sc_trace(tf, X, #X);
  VCD(clk);  VCD(rst);
  VCD(irq);  VCD(addr); VCD(rwbar);
  VCD(wdata);   VCD(rdata);
  VCD(opreq1);  VCD(opreq0);
  VCD(opack1);  VCD(opack0);
  VCD(opack);  VCD(opack);

  clock100 u_clkgen("u_clkgen");
  u_clkgen.clk(clk);


  nominalproc_rtl nominal_0("nominal_0");
  nominal_0.rst(rst);
  nominal_0.clk(clk);
  nominal_0.irq(irq);
  nominal_0.opreq(opreq);
  nominal_0.opack(opack);
  nominal_0.rdata(rdata);
  nominal_0.wdata(wdata);
  nominal_0.addr(addr);
  nominal_0.rwbar(rwbar);

  busmux busmux_0("busmux_0");
  busmux_0.threshold = 0x10000;
  busmux_0.in0(rdata0);
  busmux_0.in1(rdata1);
  busmux_0.opack0(opack0);
  busmux_0.opack1(opack1);
  busmux_0.y(rdata);
  busmux_0.opack(opack);
  busmux_0.addr(addr);


  addr_decode addr_decode_0("addr_decode_0");
  addr_decode_0.threshold = 0x10000;
  addr_decode_0.y0(opreq0);
  addr_decode_0.y1(opreq1);
  addr_decode_0.g(opreq);
  addr_decode_0.addr(addr);

  ram32 code_memory_0("code_memory_0");
  code_memory_0.contents(code, MAX);

  code_memory_0.opack(opack0);
  code_memory_0.rdata(rdata0);
  code_memory_0.opreq(opreq0);

  code_memory_0.rst(rst);
  code_memory_0.clk(clk);

  code_memory_0.wdata(wdata);
  code_memory_0.addr(addr);
  code_memory_0.rwbar(rwbar);

  ram32 code_memory_1("code_memory_1");
  code_memory_1.contents(0, MAX);

  code_memory_1.opack(opack1);
  code_memory_1.rdata(rdata1);
  code_memory_1.opreq(opreq1);

  code_memory_1.rst(rst);
  code_memory_1.clk(clk);

  code_memory_1.wdata(wdata);
  code_memory_1.addr(addr);
  code_memory_1.rwbar(rwbar);

  rst = 1; irq = 0; 
  sc_start(100, SC_NS);
  rst = 0;
  printf("End of reset\n");
  sc_start(1, SC_US);

  cout << "Single CPU SystemC nominal proc finished at " << sc_time_stamp() << "\n";
  sc_close_vcd_trace_file(tf);
  return 0;
}


// eof

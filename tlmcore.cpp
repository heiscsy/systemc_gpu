#define uint32 unsigned int
// Perhaps instead defined as sc_uint<32> ?


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "systemc.h"
#include "djip/nominalproc_tlm.h"
#include "djip/clock100.h"
#include "djip/addr_decode.h"
#include "djip/tlmram32.h"
#include "djip/tlm_busmux.h"




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
// A processor nominal unit with local instruction RAM.
//


int sc_main(int argc, char *argv[])
{


  // First processor unit (A): 

  assembler *A;
  uint32 MAX;


  sc_in <bool> rst, clk, irq;
//  sc_port <memport_if> port1;
 

  sc_signal <bool> opreq, opreq0, opreq1;
  sc_signal <uint32> addr;

  
  clock100 u_clkgen("u_clkgen");
  u_clkgen.clk(clk);



    addr_decode addr_decode_0("addr_decode_0");
    addr_decode_0.threshold = 0x10000;
    addr_decode_0.y0(opreq0);
    addr_decode_0.y1(opreq1);
    addr_decode_0.g(opreq);
    addr_decode_0.addr(addr);
    

    MAX = 128;
    uint32 *code = (uint32 *) malloc(MAX * sizeof(uint32));
    A = new assembler(code, MAX); 
    A->assemble("test_program", test_program);
    
    tlmram32 code_memory_0("code_memory_0");
    code_memory_0.contents(code, MAX);
    
    tlmram32 port1("port1");
    port1.contents(0,MAX);


    tlm_busmux busmux_0("busmux_0");
    busmux_0.threshold = 0x10000;
    busmux_0.port0(code_memory_0);
    busmux_0.port1(port1);	

    nominalproc_tlm nominal_0("nominal_0");
    nominal_0.port0(busmux_0);
    nominal_0.rst(rst);
    nominal_0.irq(irq); 
 


  irq  = 0;
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

/

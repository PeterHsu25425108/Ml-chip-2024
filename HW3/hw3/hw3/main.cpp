#include "systemc.h"
#include "clockreset.h"

int sc_main(int argc, char* argv[])
{
    // =======================
    //   signals declaration
    // =======================
    sc_signal < bool > clk;
    sc_signal < bool > rst;


    // =======================
    //   modules declaration
    // =======================
    Clock m_clock("m_clock", 10);
    Reset m_reset("m_reset", 15);
    

    // =======================
    //   modules connection
    // =======================
    m_clock( clk );
    m_reset( rst );

    
    sc_start();
    return 0;
}
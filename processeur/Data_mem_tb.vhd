--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   05:33:23 04/28/2020
-- Design Name:   
-- Module Name:   /home/ise/processeur/Data_mem_tb.vhd
-- Project Name:  processeur
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Data_mem
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY Data_mem_tb IS
END Data_mem_tb;
 
ARCHITECTURE behavior OF Data_mem_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Data_mem
    PORT(
         Addr : IN  std_logic_vector(7 downto 0);
         DATA : IN  std_logic_vector(7 downto 0);
         RW : IN  std_logic;
         RST : IN  std_logic;
         CLK : IN  std_logic;
         Qout : OUT  std_logic_vector(7 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal Addr : std_logic_vector(7 downto 0) := (others => '0');
   signal DATA : std_logic_vector(7 downto 0) := (others => '0');
   signal RW : std_logic := '0';
   signal RST : std_logic := '0';
   signal CLK : std_logic := '0';

 	--Outputs
   signal Qout : std_logic_vector(7 downto 0);

   -- Clock period definitions
   constant CLK_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Data_mem PORT MAP (
          Addr => Addr,
          DATA => DATA,
          RW => RW,
          RST => RST,
          CLK => CLK,
          Qout => Qout
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 
	Addr<= "00001011";--, "10001001" after 5*CLK_period;
	RW<= '0', '0' after CLK_period, '1' after 2*CLK_period, '1' after 3*CLK_period, '0' after 4*CLK_period;
	DATA <= "10101010"; 
	RST<= '1','1' after CLK_period ,'1' after 2*CLK_period,'0' after 10*CLK_period;

END;
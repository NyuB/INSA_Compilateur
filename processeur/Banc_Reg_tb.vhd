--------------------------------------------------------------------------------
-- Company: 
-- Engineer: DOGANIS Sensei et BRICE-sama
--
-- Create Date:   05:36:28 04/24/2020
-- Design Name:   
-- Module Name:   /home/ise/processeur/Banc_Reg_tb.vhd
-- Project Name:  processeur
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Banc_Reg
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
USE ieee.numeric_std.ALL;
 
ENTITY Banc_Reg_tb IS
END Banc_Reg_tb;
 
ARCHITECTURE behavior OF Banc_Reg_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Banc_Reg
    PORT(
         Reg_A : IN  std_logic_vector(3 downto 0);
         Reg_B : IN  std_logic_vector(3 downto 0);
         W : IN  std_logic;
         Reg_W : IN  std_logic_vector(3 downto 0);
         DATA : IN  std_logic_vector(7 downto 0);
         RST : IN  std_logic;
         CLK : IN  std_logic;
         QA : OUT  std_logic_vector(7 downto 0);
         QB : OUT  std_logic_vector(7 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal Reg_A : std_logic_vector(3 downto 0) := (others => '0');
   signal Reg_B : std_logic_vector(3 downto 0) := (others => '0');
   signal W : std_logic := '0';
   signal Reg_W : std_logic_vector(3 downto 0) := (others => '0');
   signal DATA : std_logic_vector(7 downto 0) := (others => '0');
   signal RST : std_logic := '0';
   signal CLK : std_logic := '0';

 	--Outputs
   signal QA : std_logic_vector(7 downto 0);
   signal QB : std_logic_vector(7 downto 0);

   -- Clock period definitions
   constant CLK_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Banc_Reg PORT MAP (
          Reg_A => Reg_A,
          Reg_B => Reg_B,
          W => W,
          Reg_W => Reg_W,
          DATA => DATA,
          RST => RST,
          CLK => CLK,
          QA => QA,
          QB => QB
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 
	Reg_A<= "1011", "1000" after 5*CLK_period;
	Reg_B<= "0011", "1000" after 5*CLK_period;
	W<= '1', '1' after CLK_period, '1' after 2*CLK_period, '0' after 3*CLK_period, '0' after 4*CLK_period;
	DATA <= "10101010"; 
	Reg_W<= "0011", "1111" after CLK_period;
	RST<= '1','1' after CLK_period ,'1' after 2*CLK_period,'0' after 10*CLK_period;
END;

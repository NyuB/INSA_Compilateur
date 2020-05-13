--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   07:59:01 04/30/2020
-- Design Name:   
-- Module Name:   /home/ise/processeur/pipes_tb.vhd
-- Project Name:  processeur
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Pipes
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
use IEEE.STD_LOGIC_UNSIGNED.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
USE ieee.numeric_std.ALL;
 
ENTITY pipes_tb IS
END pipes_tb;
 
ARCHITECTURE behavior OF pipes_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Pipes
    PORT(
         CLK : IN  std_logic;
         IP : IN  std_logic_vector(7 downto 0);
         SA : OUT  std_logic_vector(7 downto 0);
         SOP : OUT  std_logic_vector(7 downto 0);
         SB : OUT  std_logic_vector(7 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal CLK : std_logic := '0';
   signal IP : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal SA : std_logic_vector(7 downto 0);
   signal SOP : std_logic_vector(7 downto 0);
   signal SB : std_logic_vector(7 downto 0);

   -- Clock period definitions
   constant CLK_period : time := 1000 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Pipes PORT MAP (
          CLK => CLK,
          IP => IP,
          SA => SA,
          SOP => SOP,
          SB => SB
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 
process(CLK)
begin
	if rising_edge(CLK) then
		if IP<X"13" then
			IP <= IP+X"01";
		end if;
	end if;
end process;

END;

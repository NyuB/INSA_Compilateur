----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    05:28:01 04/28/2020 
-- Design Name: 
-- Module Name:    Instruct_mem - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Instruct_mem is
    Port ( Addr : in  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC;
           Qout : out  STD_LOGIC_VECTOR (31 downto 0));
end Instruct_mem;

architecture Behavioral of Instruct_mem is
type memory_array is array(integer range 255 downto 0) of std_logic_vector(31 downto 0);
signal memory : memory_array;
begin
memory(0) <= X"00060000";
memory(1) <= X"01060100";
memory(2) <= X"02060200";
memory(3) <= X"03060300";
memory(4) <= X"04060400";
memory(5) <= X"05050100";
memory(6) <= X"06020202";
memory(7) <= X"07020301";
memory(8) <= X"08020302";
--------------------------
memory(9) <= X"01080100";
memory(10) <= X"02080200";

memory(11) <= X"03070100";
memory(12) <= X"04070200";
memory(13) <= X"00060000";
memory(14) <= X"00060000";
memory(15) <= X"00060000";
memory(16) <= X"09010304";
memory(17) <= X"00060000";

process(CLK)
begin
	if rising_edge(CLK) then
		Qout <= memory(to_integer(unsigned(Addr)));
	--previous work
		--Qout(31 downto 24)<= memory(to_integer(unsigned(Addr)));
		--Qout(23 downto 16)<= memory(to_integer(unsigned(Addr))+1);
		--Qout(15 downto 8)<= memory(to_integer(unsigned(Addr))+2);
		--Qout(7 downto 0)<= memory(to_integer(unsigned(Addr))+3);
		--quelques tests
		--Qout <= X"01020304";
		--Qout <= X"01060100";
		--Qout <= X"02050100";
	end if;
end process;

end Behavioral;


----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    05:08:08 04/28/2020 
-- Design Name: 
-- Module Name:    Data_mem - Behavioral 
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
--use IEEE.STD_LOGIC_ARITH.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Data_mem is
    Port ( Addr : in  STD_LOGIC_VECTOR (7 downto 0);
           DATA : in  STD_LOGIC_VECTOR (7 downto 0);
           RW : in  STD_LOGIC;
           RST : in  STD_LOGIC;
           CLK : in  STD_LOGIC;
           Qout : out  STD_LOGIC_VECTOR (7 downto 0));
end Data_mem;

architecture Behavioral of Data_mem is
type memory_array is array(integer range 255 downto 0) of std_logic_vector(7 downto 0);
signal memory : memory_array;

begin

process(CLK)
begin
	if falling_edge(CLK) then
		if RST='0' then
			memory<=(OTHERS => X"00");
		end if;
		--lecture
		if RW='1' then
			Qout<= memory(to_integer(unsigned(Addr)));
		end if;
		--ecriture
		if RW='0' then
			memory(to_integer(unsigned(Addr))) <= DATA;
		end if;
	end if;

end process;

end Behavioral;


----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    05:09:27 04/24/2020 
-- Design Name: 
-- Module Name:    Banc_Reg - Behavioral 
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

entity Banc_Reg is
    Port ( Reg_A : in  STD_LOGIC_VECTOR (3 downto 0);
           Reg_B : in  STD_LOGIC_VECTOR (3 downto 0);
           W : in  STD_LOGIC;
           Reg_W : in  STD_LOGIC_VECTOR (3 downto 0);
           DATA : in  STD_LOGIC_VECTOR (7 downto 0);
           RST : in  STD_LOGIC;
           CLK : in  STD_LOGIC;
           QA : out  STD_LOGIC_VECTOR (7 downto 0);
           QB : out  STD_LOGIC_VECTOR (7 downto 0));
end Banc_Reg;

architecture Behavioral of Banc_Reg is
type registers_array is array(integer range 15 downto 0) of std_logic_vector(7 downto 0);
signal registers : registers_array;

begin
	QA<= registers(to_integer(unsigned(Reg_A)));
	QB<= registers(to_integer(unsigned(Reg_B)));
process(CLK)
begin
	if rising_edge(CLK) then
		if RST='0' then
			registers <= (OTHERS => X"00");
		end if;
		if W='1' then
			registers(to_integer(unsigned(Reg_W))) <= DATA;
		end if;
	end if;
end process;

end Behavioral;


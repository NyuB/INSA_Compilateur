----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    06:43:56 04/30/2020 
-- Design Name: 
-- Module Name:    LIDI - Behavioral 
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

entity LIDI is
    Port ( A : in  STD_LOGIC_VECTOR (7 downto 0);
           B : in  STD_LOGIC_VECTOR (7 downto 0);
           C : in  STD_LOGIC_VECTOR (7 downto 0);
           OP : in  STD_LOGIC_VECTOR (7 downto 0);
           QA : out  STD_LOGIC_VECTOR (7 downto 0);
           QB : out  STD_LOGIC_VECTOR (7 downto 0);
           QC : out  STD_LOGIC_VECTOR (7 downto 0);
           QOP : out  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC);
end LIDI;

architecture Behavioral of LIDI is

begin
process(CLK)
begin
	if rising_edge(CLK) then
		QA<= A;
		QOP <= OP;
		QB <= B;
		QC <= C;
	end if;
end process;

end Behavioral;


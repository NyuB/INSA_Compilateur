----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Brice sama
-- 
-- Create Date:    09:41:50 04/21/2020 
-- Design Name: 
-- Module Name:    ALU - Behavioral 
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
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ALU is
    Port ( A : in  STD_LOGIC_VECTOR (7 downto 0);
           B : in  STD_LOGIC_VECTOR (7 downto 0);
           N : out  STD_LOGIC;
           O : out  STD_LOGIC;
           Z : out  STD_LOGIC;
           C : out  STD_LOGIC;
           Ctrl : in  STD_LOGIC_VECTOR (2 downto 0);
           S : out  STD_LOGIC_VECTOR (7 downto 0));
end ALU;

architecture Behavioral of ALU is
signal aux_S : STD_LOGIC_VECTOR (31 downto 0);
signal aux_N : STD_LOGIC;
signal aux_O : STD_LOGIC;
signal aux_Z : STD_LOGIC;
signal aux_C : STD_LOGIC;
signal aux_A : STD_LOGIC_VECTOR (15 downto 0);
signal aux_B : STD_LOGIC_VECTOR (15 downto 0);
begin
	s<=aux_S(7 downto 0);
	aux_A <= "00000000" & A;
	aux_B <= "00000000" & B;
	process(aux_A,aux_B,Ctrl)
	begin
	N<='0';
	O<='0';
	Z<='0';
	C<='0';
			if Ctrl = "001" then
				aux_S(15 downto 0) <= aux_A+aux_B;
				if aux_A+aux_B > X"000000FF" then --255
					C <= '1';
				end if;	
				if aux_A+aux_B(7 downto 0) > X"7F" then --127
					N<='1';
				end if;	
				if aux_A+aux_B(7 downto 0) = X"00" then  --0
					Z<='1';
				end if;	
			end if;
			
			
		if Ctrl = "011" then
				aux_S(15 downto 0) <= aux_A-aux_B;
				if aux_A-aux_B > X"000000FF" then --255
				C <= '1';
				end if;	
				if aux_A-aux_B(7 downto 0) > X"7F" then --127
					N<='1';
				end if;	
				if aux_A-aux_B(7 downto 0) = X"00" then  --0
					Z<='1';
				end if;	
			end if;
		
		
		
		if Ctrl = "010" then
			aux_S <= aux_A*aux_B;
			if aux_A*aux_B > X"000000FF" then --255
				O <= '1';
			end if;	
			if aux_A*aux_B(7 downto 0) > X"7F" then --127
				N<='1';
			end if;
			if aux_A*aux_B(7 downto 0) = X"00" then --0
				Z<='1';
			end if;
			
		end if;
		
		--on fait pas la division
	end process;
end Behavioral;


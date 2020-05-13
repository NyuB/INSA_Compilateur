----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    06:49:16 04/30/2020 
-- Design Name: 
-- Module Name:    Pipes - Behavioral 
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

entity Pipes is
    Port ( CLK : in  STD_LOGIC;
           IP : in  STD_LOGIC_VECTOR (7 downto 0);
			  SA : out STD_LOGIC_VECTOR (7 downto 0);
			  SOP : out STD_LOGIC_VECTOR (7 downto 0);
			  SB : out STD_LOGIC_VECTOR (7 downto 0)
           );
end Pipes;

architecture Behavioral of Pipes is
component LIDI is
port(A : in  STD_LOGIC_VECTOR (7 downto 0);
           B : in  STD_LOGIC_VECTOR (7 downto 0);
           C : in  STD_LOGIC_VECTOR (7 downto 0);
           OP : in  STD_LOGIC_VECTOR (7 downto 0);
           QA : out  STD_LOGIC_VECTOR (7 downto 0);
           QB : out  STD_LOGIC_VECTOR (7 downto 0);
           QC : out  STD_LOGIC_VECTOR (7 downto 0);
           QOP : out  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC);
end component LIDI;

component Banc_Reg is
port(Reg_A : IN  std_logic_vector(3 downto 0);
         Reg_B : IN  std_logic_vector(3 downto 0);
         W : IN  std_logic;
         Reg_W : IN  std_logic_vector(3 downto 0);
         DATA : IN  std_logic_vector(7 downto 0);
         RST : IN  std_logic;
         CLK : IN  std_logic;
         QA : OUT  std_logic_vector(7 downto 0);
         QB : OUT  std_logic_vector(7 downto 0));
end component Banc_Reg;

component ALU is
port(A : IN  std_logic_vector(7 downto 0);
         B : IN  std_logic_vector(7 downto 0);
         N : OUT  std_logic;
         O : OUT  std_logic;
         Z : OUT  std_logic;
         C : OUT  std_logic;
         Ctrl : IN  std_logic_vector(2 downto 0);
         S : OUT  std_logic_vector(7 downto 0));
end component ALU;

component EXMEM is
port(A : in  STD_LOGIC_VECTOR (7 downto 0);
           OP : in  STD_LOGIC_VECTOR (7 downto 0);
           B : in  STD_LOGIC_VECTOR (7 downto 0);
           QA : out  STD_LOGIC_VECTOR (7 downto 0);
           QOP : out  STD_LOGIC_VECTOR (7 downto 0);
           QB : out  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC);
end component EXMEM;

component Data_mem is
port(Addr : IN  std_logic_vector(7 downto 0);
         DATA : IN  std_logic_vector(7 downto 0);
         RW : IN  std_logic;
         RST : IN  std_logic;
         CLK : IN  std_logic;
         Qout : OUT  std_logic_vector(7 downto 0));
end component Data_mem;

component Instruct_mem is
port(Addr : in  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC;
           Qout : out  STD_LOGIC_VECTOR (31 downto 0));
end component Instruct_mem;

--MEMI
signal memiO : STD_LOGIC_VECTOR (31 downto 0);
--MEMRE
signal memreA : STD_LOGIC_VECTOR (7 downto 0);
signal memreB : STD_LOGIC_VECTOR (7 downto 0);
signal memreOP : STD_LOGIC_VECTOR (7 downto 0);
--LIDI
signal lidiA : STD_LOGIC_VECTOR (7 downto 0);
signal lidiB : STD_LOGIC_VECTOR (7 downto 0);
signal lidiC : STD_LOGIC_VECTOR (7 downto 0);
signal lidiOP : STD_LOGIC_VECTOR (7 downto 0);
--DIEX
signal diexA : STD_LOGIC_VECTOR (7 downto 0);
signal diexB : STD_LOGIC_VECTOR (7 downto 0);
signal diexC : STD_LOGIC_VECTOR (7 downto 0);
signal diexOP : STD_LOGIC_VECTOR (7 downto 0);
--EXMEM
signal exmemA : STD_LOGIC_VECTOR (7 downto 0);
signal exmemB : STD_LOGIC_VECTOR (7 downto 0);
signal exmemOP : STD_LOGIC_VECTOR (7 downto 0);
--banc reg
signal regA : STD_LOGIC_VECTOR (7 downto 0);
signal regB : STD_LOGIC_VECTOR (7 downto 0);
--ALU
signal sortieS : STD_LOGIC_VECTOR (7 downto 0);
signal flagN : STD_LOGIC;
signal flagO : STD_LOGIC;
signal flagZ : STD_LOGIC;
signal flagC : STD_LOGIC;
--memoire donnes
signal sortieOUT : STD_LOGIC_VECTOR (7 downto 0);
signal toAddr : STD_LOGIC_VECTOR (7 downto 0);
signal toRW : STD_LOGIC;
--multiplexeurs
signal muxlidi : STD_LOGIC_VECTOR (7 downto 0);
signal muxdiex : STD_LOGIC_VECTOR (7 downto 0);
signal muxmemreB : STD_LOGIC_VECTOR (7 downto 0);
--to write
signal toW : STD_LOGIC;
 

begin
 
process(lidiOP, lidiB, regA, memreOP, diexOP, diexB, sortieS, sortieOUT, memreA, memreB, exmemOP, exmemA, exmemB)
begin
	muxmemreB <= exmemB;
	toW <= '0';
	muxlidi <= lidiB;

	if lidiOP = X"01" or lidiOP = X"02" or lidiOP = X"03" or lidiOP = X"05" or lidiOP = X"08" then
		muxlidi <= regA;
	end if;
	if diexOP = X"06" or diexOP=X"05" or diexOP=X"07" or diexOP=X"08" then
		muxdiex <= diexB;
	end if;
	if diexOP = X"01" or diexOP=X"02" or diexOP = X"03" then
		muxdiex <= sortieS;
	end if;
	if memreOP = X"01" or memreOP = X"02" or memreOP = X"03" or memreOP = X"05" or memreOP = X"06" or memreOP = X"07" then
		toW <= '1';
	end if;
	if exmemOP = X"07" then
		toRW <= '1';
		toAddr <= exmemB;
		muxmemreB <= sortieOUT;
	end if;
	if exmemOP = X"08" then
		toRW <= '0';
		toAddr <= exmemA;
	end if;
	
end process;





--sorties debug
--SA<= memreA;
--SB<= memreB;
--SOP<= memreOP;


MEMI : Instruct_mem port map (
CLK => CLK,
Addr => IP,
Qout => memiO 
);

LIDIP : LIDI port map(
CLK => CLK,
A => memiO (31 downto 24),
OP => memiO (23 downto 16),
B => memiO (15 downto 8),
C => memiO (7 downto 0),
QA => lidiA ,
QC => lidiC ,
QB => lidiB ,
QOP => lidiOP 

);

BANC : Banc_Reg port map (
RST => '1',
CLK => CLK,
Reg_A => lidiB(3 downto 0),--X"0",
Reg_B => lidiC(3 downto 0),
QA => regA,--diexC,
QB => regB,
Reg_W => memreA (3 downto 0),
W => toW,
DATA => memreB
);

DIEXP : LIDI port map (
CLK => CLK,
A => lidiA ,
OP => lidiOP,
B => muxlidi,
C => regB,
QA => diexA ,
QC => diexC,
QB => diexB,
QOP => diexOP
);

UAL : ALU port map (
A => diexB,
B => diexC,
N => flagN,
O => flagO,
Z => flagZ,
C => flagC,
Ctrl => diexOP(2 downto 0),
S => sortieS
);

EXMEMP : EXMEM port map (
CLK => CLK,
A => diexA ,
B => muxdiex ,
OP => diexOP,
QA => exmemA ,
QB => exmemB,
QOP => exmemOP 
);

MEMD : Data_mem port map (
Addr => toAddr,
DATA => exmemB,
RW => toRW ,
RST => '1',
CLK => CLK,
Qout => sortieOUT
);

MEMREP : EXMEM port map(
CLK => CLK,
A => exmemA ,
B => muxmemreB ,
OP => exmemOP ,
QA => memreA,
QB => memreB ,
QOP => memreOP 
);


end Behavioral;


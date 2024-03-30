#include "syscall.h"
#include "copyright.h"

int main()
{
	int openFileID = Open("ascii.txt", 0);
	int i = 0;
	PrintString("\n\n----ASCII TABLE----\n\n");
	
	PrintString("0	NUL	Null\n1	01	00000001	&#1;	SOH	Start of Heading\n2	02	00000010	&#2;	STX	Start of Text\n3	03	00000011	&#3;	ETX	End of Text\n4	04	00000100	&#4;	EOT	End of Transmission\n5	05	00000101	&#5;	ENQ	Enquiry\n6	06	00000110	&#6;	ACK	Acknowledge\n");
	Write("0	00	00000000	&#0;	NUL	Null\n1	01	00000001	&#1;	SOH	Start of Heading\n2	02	00000010	&#2;	STX	Start of Text\n3	03	00000011	&#3;	ETX	End of Text\n4	04	00000100	&#4;	EOT	End of Transmission\n5	05	00000101	&#5;	ENQ	Enquiry\n6	06	00000110	&#6;	ACK	Acknowledge\n", 249, openFileID);
	
	PrintString("7	07	00000111	&#7;	BEL	Bell\n8	08	00001000	&#8;	BS	Backspace\n9	09	00001001	&#9;	HT	Horizontal Tab\n10	0A	00001010	&#10;	LF	Line Feed\n11	0B	00001011	&#11;	VT	Vertical Tab\n12	0C	00001100	&#12;	FF	Form Feed\n13	0D	00001101	&#13;	CR	Carriage Return\n");
	Write("7	07	00000111	&#7;	BEL	Bell\n8	08	00001000	&#8;	BS	Backspace\n9	09	00001001	&#9;	HT	Horizontal Tab\n10	0A	00001010	&#10;	LF	Line Feed\n11	0B	00001011	&#11;	VT	Vertical Tab\n12	0C	00001100	&#12;	FF	Form Feed\n13	0D	00001101	&#13;	CR	Carriage Return\n", 242, openFileID);
	
	PrintString("14	0E	00001110	&#14;	SO	Shift Out\n15	0F	00001111	&#15;	SI	Shift In\n16	10	00010000	&#16;	DLE	Data Link Escape\n17	11	00010001	&#17;	DC1	Device Control 1\n18	12	00010010	&#18;	DC2	Device Control 2\n19	13	00010011	&#19;	DC3	Device Control 3\n");
	Write("14	0E	00001110	&#14;	SO	Shift Out\n15	0F	00001111	&#15;	SI	Shift In\n16	10	00010000	&#16;	DLE	Data Link Escape\n17	11	00010001	&#17;	DC1	Device Control 1\n18	12	00010010	&#18;	DC2	Device Control 2\n19	13	00010011	&#19;	DC3	Device Control 3\n", 235, openFileID);
	
	PrintString("20	14	00010100	&#20;	DC4	Device Control 4\n21	15	00010101	&#21;	NAK	Negative Acknowledge\n22	16	00010110	&#22;	SYN	Synchronize\n23	17	00010111	&#23;	ETB	End of Transmission Block\n24	18	00011000	&#24;	CAN	Cancel\n25	19	00011001	&#25;	EM	End of Medium\n26	1A	00011010	&#26;	SUB	Substitute\n");
	
	PrintString("26	1A	00011010	&#26;	SUB	Substitute\n27	1B	00011011	&#27;	ESC	Escape\n28	1C	00011100	&#28;	FS	File Separator\n29	1D	00011101	&#29;	GS	Group Separator\n30	1E	00011110	&#30;	RS	Record Separator\n31	1F	00011111	&#31;	US	Unit Separator\n");
	
	PrintString("32	20	00100000	&#32;	space	Space\n33	21	00100001	&#33;	!	exclamation mark\n34	22	00100010	&#34;	\"	double quote\n35	23	00100011	&#35;	#	number\n36	24	00100100	&#36;	$	dollar\n37	25	00100101	&#37;	%	percent\n38	26	00100110	&#38;	&	ampersand\n");
	
	PrintString("39	27	00100111	&#39;	'	single quote\n40	28	00101000	&#40;	(	left parenthesis\n41	29	00101001	&#41;	)	right parenthesis\n42	2A	00101010	&#42;	*	asterisk\n43	2B	00101011	&#43;	+	plus\n44	2C	00101100	&#44;	,	comma\n45	2D	00101101	&#45;	-	minus\n");

	PrintString("46	2E	00101110	&#46;	.	period\n47	2F	00101111	&#47;	/	slash\n48	30	00110000	&#48;	0	zero\n49	31	00110001	&#49;	1	one\n50	32	00110010	&#50;	2	two\n");

	PrintString("51	33	00110011	&#51;	3	three\n52	34	00110100	&#52;	4	four\n53	35	00110101	&#53;	5	five\n54	36	00110110	&#54;	6	six\n55	37	00110111	&#55;	7	seven\n56	38	00111000	&#56;	8	eight\n57	39	00111001	&#57;	9	nine\n58	3A	00111010	&#58;	:	colon\n");

	PrintString("59	3B	00111011	&#59;	;	semicolon\n60	3C	00111100	&#60;	<	less than\n61	3D	00111101	&#61;	=	equality sign\n62	3E	00111110	&#62;	>	greater than\n63	3F	00111111	&#63;	?	question mark\n64	40	01000000	&#64;	@	at sign\n");
	
	Yield();
	
	PrintString("65	41	01000001	&#65;	A	 \n66	42	01000010	&#66;	B	 \n67	43	01000011	&#67;	C	 \n68	44	01000100	&#68;	D	 \n69	45	01000101	&#69;	E	 \n70	46	01000110	&#70;	F	 \n71	47	01000111	&#71;	G	 \n72	48	01001000	&#72;	H	 \n73	49	01001001	&#73;	I	 \n74	4A	01001010	&#74;	J	 \n75	4B	01001011	&#75;	K	 \n");
	
	
	Close(openFileID);
	return 0;
}
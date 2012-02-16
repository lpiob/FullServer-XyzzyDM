// gangzones.inc
// definicje stref rysowanych na radarze + funkcje rysujace je dla gracza

#define GZ_InitGZ(%0) \
	for(new i=0;i<sizeof %0; i++)	\
		%0[i][egzgz]=GangZoneCreate( %0[i][egzx1], %0[i][egzy1],%0[i][egzx2],%0[i][egzy2]);



#define GZ_DrawForPlayer(%0,%1) \
	for(new i=0;i<sizeof %1; i++) { \
		GangZoneShowForPlayer(%0,%1[i][egzgz],%1[i][egzcolor]); \
	}


enum gzdef{
	Float:egzx1,Float:egzy1,
	Float:egzx2,Float:egzy2,
	egzcolor, egzgz
}

new gz_arena_aztec[][gzdef]={	// /areny -> /aztec
	{-5033.9, -33.9,	-4966.0, 33.9,	0x000000cc},// aztecall
	{-5026.8, -26.7,	-4973.2, 26.8,	0x30309090},	// azteccenter
	{-5030.4, -4.4,	-4969.5, 4.5,	0x90909090},	// aztec1
	{-5004.4, -30.2,	-4995.5, 30.5,	0x90909090} 	// aztec2
};


new gz_miasto1[][gzdef]={

{-458.6, 5494.1,	-161.8, 5584.7,	0x808080FF},	// port1
{-458.2, 5559.0,	-447.1, 5708.9,	0x808080FF},	// port2
{-293.9, 5573.6,	-96.8, 5736.0,	0x808080FF},	// port2
{-450.2, 5213.3,	-246.9, 5400.2,	0x808080FF},	// port3

{-136.81, 2065.5,	-121.8, 5343.3,	0x000000FF},	// ulica
{-157.2, 2065.5,	-140.13, 5343.3,	0x000000FF},	// +ulica

{-305.4, 4639.8,	179.7, 4654.03,	0x000000FF},	// ulica2
{-305.4, 4659.05,	179.7, 4676.6,	0x000000FF},	// ulica2

{-305.1, 3958.5,	-288.5, 4869.3,	0x000000FF}, 	// ulica3

{-771.0, 4850.1,	-207.0, 4865.1,	0x000000FF},	// ulica5
{-652.1, 4864.7,	-641.7, 5130.6,	0x000000FF}, 	// ulica5
{-737.8, 5128.2,	-409.2, 5138.2,	0x000000FF},	// ulica

{-461.8, 4974.8,	-383.1, 4990.3,	0x000000FF},	// uliczka
{-462.1, 4930.4,	-451.7, 4990.7,	0x000000FF},	// uliczka
{-467.5, 4935.0,	-331.8, 4944.5,	0x000000FF},	// uliczka

{-342.3, 4863.3,	-331.9, 4944.3,	0x000000FF},	// uliczka
{-268.1, 4864.5,	-252.2, 5292.4,	0x000000FF},	// ulica

{-421.3, 5282.9,	-252.1, 5296.2,	0x000000FF},	// uliczka	port
{-421.3, 5284.0,	-411.6, 5550.0,	0x000000FF},	// uliczka	port
{-421.5, 5540.1,	-202.8, 5550.2,	0x000000FF},	// uliczka port
{-253.7, 5544.9,	-243.6, 5727.7,	0x000000FF},	// uliczka port

{-183.9, 4427.7,	-169.9, 4645.4,	0x000000FF},	// uliczka
{-521.1, 4563.1,	-180.7, 4582.0,	0x000000FF},	// ulica


{-418.7, 4989.8,	-408.5, 5133.8,	0x000000FF}	// ulica


};


GZ_Init(){
	GZ_InitGZ(gz_arena_aztec)
	GZ_InitGZ(gz_miasto1)
/*#define gz gz_arena_aztec
	for(new i=0;i<sizeof gz; i++) {
		gz[i][egzgz]=GangZoneCreate( gz[i][egzx1], gz[i][egzy1],gz[i][egzx2],gz[i][egzy2]);
	}
#undef gz*/
}



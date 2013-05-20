
#define GET_PLL_P(x)		((x>>20) & 0x3f)
#define GET_PLL_M(x)		((x>>8) & 0x3ff)
#define GET_PLL_S(x)		(x & 7)


typedef unsigned long ulong;  

void raise(void)
{

}

ulong get_clock_arm(void)
{
	return 1000000000;
}

static ulong echo_p_calc_bus_pms(unsigned int pms)
{
	
	unsigned int	p = GET_PLL_P(pms);
	unsigned int 	m = GET_PLL_M(pms);
	unsigned int 	s = GET_PLL_S(pms);
	*(volatile unsigned int*)0xa0000000 = 0;
	*(volatile unsigned int*)0xa0000000 = (m* ((24000000 >> s) / p));
	return (m* ((24000000 >> s) / p));
}


ulong get_clock_bus(void)
{
	unsigned int pms_bus = *(volatile unsigned int*)0x30090828;
	
	return echo_p_calc_bus_pms(pms_bus);
}

/* from micom */
ulong get_clock_apb(void)
{
	return (get_clock_bus() / 2);
}

int print_cpuinfo(void)
{
	int cpu_id = (*(volatile unsigned int*)0x300000C0);

	ulong arm_clock = get_clock_arm();
	ulong bus_clock = get_clock_bus();
	ulong apb_clock = get_clock_apb();

//	printf("SDP1001 #%d Operation clock\n", cpu_id);
//	printf("arm clock :  %03d.%03d Mhz\n", (int)arm_clock/1000000, (int)(arm_clock%1000000)/10000);
//	printf("bus clock :  %03d.%03d Mhz\n", (int)bus_clock/1000000, (int)(bus_clock%1000000)/10000);
//	printf("peri clock : %03d.%03d Mhz\n", (int)apb_clock/1000000, (int)(apb_clock%1000000)/10000);

	return 0;
}

#ifndef _SAME70N19_SYSCLK_H_
#define _SAME70N19_SYSCLK_H_

#include <stdint.h>
#include "same70n19.h"

void sysclk_set_prescaler(uint32_t prescaler)
{
	// Configure prescaler value
	PMC.PMC_MCKR = (PMC.PMC_MCKR & ~PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES(prescaler);
	// Wait for Master Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MCKRDY));
}

void sysclk_set_divider(uint32_t div)
{
	// Configure divider value
	PMC.PMC_MCKR = (PMC.PMC_MCKR & ~PMC_MCKR_MDIV_Msk) | PMC_MCKR_MDIV(div);
	// Wait for Master Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MCKRDY));	
}

void sysclk_init_osc32krc(void)
{
	// Configure slow clock to Master Clock
	PMC.PMC_MCKR &= ~PMC_MCKR_CSS_Msk;
	// Wait for Master Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MCKRDY));
}

void sysclk_bypass_osc32kxtal(void)
{
	// Configure OSC32K bypass
	SUPC.SUPC_MR |= SUPC_MR_KEY_PASSWD | SUPC_MR_OSCBYPASS;
}

void sysclk_init_osc32kxtal(void)
{
	// Enable OSC32K xtal
	SUPC.SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_XTALSEL;
	// Wait for OSC32K xtal to be ready
	while(!((SUPC.SUPC_SR & SUPC_SR_OSCSEL) && (PMC.PMC_SR & PMC_SR_OSCSELS)));
	// Configure slow clock to Master Clock
	PMC.PMC_MCKR &= ~PMC_MCKR_CSS_Msk;
	// Wait for Master Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MCKRDY));
}

void sysclk_init_osc8mrc(void)
{
	// Enable Fast RC
	PMC.CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);
	// Wait for Fast RC to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCRCS));
	// Change Fast RC frequency to 8MHz
	PMC.CKGR_MOR = (PMC.CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCF_8_MHz;
	// Wait for Fast RC to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCRCS));
	// Configure Main Clock to use Fast RC
	PMC.CKGR_MOR = (PMC.CKGR_MOR & ~CKGR_MOR_MOSCSEL) | CKGR_MOR_KEY_PASSWD;
	// Wait for Main Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCSELS));
	// Configure Main Clock to Master Clock
	PMC.PMC_MCKR = (PMC.PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	// Wait for Master Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MCKRDY));
}

void sysclk_init_osc12mrc(void)
{
	// Enable Fast RC
	PMC.CKGR_MOR |= (CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCEN);
	// Wait for Fast RC to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCRCS));
	// Change Fast RC frequency to 12MHz
	PMC.CKGR_MOR = (PMC.CKGR_MOR & ~CKGR_MOR_MOSCRCF_Msk) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCRCF_12_MHz;
	// Wait for Fast RC to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCRCS));
	// Configure Main Clock to use Fast RC
	PMC.CKGR_MOR = (PMC.CKGR_MOR & ~CKGR_MOR_MOSCSEL) | CKGR_MOR_KEY_PASSWD;
	// Wait for Main Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCSELS));
	// Configure Main Clock to Master Clock
	PMC.PMC_MCKR = (PMC.PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	// Wait for Master Clock to be ready
	while(!(PMC.PMC_SR & PMC_SR_MCKRDY));
}

void sysclk_bypass_xtal(void)
{
	// Disable Main Clock oscillator, enable bypass, and 
	PMC.CKGR_MOR = (PMC.CKGR_MOR & ~CKGR_MOR_MOSCXTEN) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTBY;
	// Wait for Main Clock oscillator to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCXTS));
}

void sysclk_init_xtal(void)
{
	// Disable Main Clock bypass, enable oscillator, and set startup time
	PMC.CKGR_MOR = (PMC.PMC_MCKR & ~CKGR_MOR_MOSCXTBY) | CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCXTST(0x40);
	// Wait for Main Clock oscillator to be ready
	while(!(PMC.PMC_SR & PMC_SR_MOSCXTS));
	// Configure Main Clock to use xtal
	PMC.CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
}

void sysclk_init_pll(uint32_t div, uint32_t mul)
{
	// Disable PLL
	PMC.CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(0);
	// Set PLL Multiplier, Divider, and PLL lock delay
	PMC.CKGR_PLLAR = CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(mul - 1) | CKGR_PLLAR_DIVA(div) | CKGR_PLLAR_PLLACOUNT(0x3F);
	// Wait for PLL lock
	while(!(PMC.PMC_SR & PMC_SR_LOCKA));
	// Configure Main Clock to use PLL
	PMC.PMC_MCKR = (PMC.PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_PLLA_CLK;
}

void sysclk_init_utmi_pll(void)
{
	// Set UTMI PLL count and enable it
	PMC.CKGR_UCKR = CKGR_UCKR_UPLLCOUNT(0x3F) | CKGR_UCKR_UPLLEN;
	// Wait for UTMI PLL to lock
	while(!(PMC.PMC_SR & PMC_SR_LOCKU));
	// Configure Main Clock to use UTMI PLL
	PMC.PMC_MCKR = (PMC.PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_UPLL_CLK;
}

#endif

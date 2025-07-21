#include <furi_hal_bus.h>
#include <furi.h>
#include <array>
#include <stm32wbxx_ll_bus.h>

/* Bus bitmask definitions */

#define FURI_HAL_BUS_AHB1_GRP1                                                           \
    (LL_AHB1_GRP1_PERIPH_DMA1 | LL_AHB1_GRP1_PERIPH_DMA2 | LL_AHB1_GRP1_PERIPH_DMAMUX1 | \
     LL_AHB1_GRP1_PERIPH_CRC | LL_AHB1_GRP1_PERIPH_TSC)

#if defined(ADC_SUPPORT_5_MSPS)
#define FURI_HAL_BUS_AHB2_GRP1                                                           \
    (LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOB | LL_AHB2_GRP1_PERIPH_GPIOC | \
     LL_AHB2_GRP1_PERIPH_GPIOD | LL_AHB2_GRP1_PERIPH_GPIOE | LL_AHB2_GRP1_PERIPH_GPIOH | \
     LL_AHB2_GRP1_PERIPH_ADC | LL_AHB2_GRP1_PERIPH_AES1)

#define FURI_HAL_BUS_APB2_GRP1                                                          \
    (LL_APB2_GRP1_PERIPH_TIM1 | LL_APB2_GRP1_PERIPH_SPI1 | LL_APB2_GRP1_PERIPH_USART1 | \
     LL_APB2_GRP1_PERIPH_TIM16 | LL_APB2_GRP1_PERIPH_TIM17 | LL_APB2_GRP1_PERIPH_SAI1)
#else
#define FURI_HAL_BUS_AHB2_GRP1                                                           \
    (LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOB | LL_AHB2_GRP1_PERIPH_GPIOC | \
     LL_AHB2_GRP1_PERIPH_GPIOD | LL_AHB2_GRP1_PERIPH_GPIOE | LL_AHB2_GRP1_PERIPH_GPIOH | \
     LL_AHB2_GRP1_PERIPH_AES1)

#define FURI_HAL_BUS_APB2_GRP1                                                            \
    (LL_APB2_GRP1_PERIPH_ADC | LL_APB2_GRP1_PERIPH_TIM1 | LL_APB2_GRP1_PERIPH_SPI1 |      \
     LL_APB2_GRP1_PERIPH_USART1 | LL_APB2_GRP1_PERIPH_TIM16 | LL_APB2_GRP1_PERIPH_TIM17 | \
     LL_APB2_GRP1_PERIPH_SAI1)
#endif

#define FURI_HAL_BUS_AHB3_GRP1                                                          \
    (LL_AHB3_GRP1_PERIPH_QUADSPI | LL_AHB3_GRP1_PERIPH_PKA | LL_AHB3_GRP1_PERIPH_AES2 | \
     LL_AHB3_GRP1_PERIPH_RNG | LL_AHB3_GRP1_PERIPH_HSEM | LL_AHB3_GRP1_PERIPH_IPCC)
//   LL_AHB3_GRP1_PERIPH_FLASH enabled by default

#define FURI_HAL_BUS_APB1_GRP1                                                       \
    (LL_APB1_GRP1_PERIPH_TIM2 | LL_APB1_GRP1_PERIPH_LCD | LL_APB1_GRP1_PERIPH_SPI2 | \
     LL_APB1_GRP1_PERIPH_I2C1 | LL_APB1_GRP1_PERIPH_I2C3 | LL_APB1_GRP1_PERIPH_CRS | \
     LL_APB1_GRP1_PERIPH_USB | LL_APB1_GRP1_PERIPH_LPTIM1)

#define FURI_HAL_BUS_APB1_GRP2 (LL_APB1_GRP2_PERIPH_LPUART1 | LL_APB1_GRP2_PERIPH_LPTIM2)
#define FURI_HAL_BUS_APB3_GRP1 (LL_APB3_GRP1_PERIPH_RF)

enum class Bus : uint8_t {
    AHB1,
    AHB2,
    AHB3,
    APB1,
    APB2,
    APB3,
};

enum class BusGroup : uint8_t {
    GRP1,
    GRP2
};

struct BusInfo {
    Bus bus;
    BusGroup group;
    uint32_t periph;
};

static constexpr BusInfo bus_table[] = {
    /*FuriHalBusDMA1*/ {Bus::AHB1, BusGroup::GRP1, LL_AHB1_GRP1_PERIPH_DMA1},
    /*FuriHalBusDMA2*/ {Bus::AHB1, BusGroup::GRP1, LL_AHB1_GRP1_PERIPH_DMA2},
    /*FuriHalBusDMAMUX1*/ {Bus::AHB1, BusGroup::GRP1, LL_AHB1_GRP1_PERIPH_DMAMUX1},
    /*FuriHalBusCRC*/ {Bus::AHB1, BusGroup::GRP1, LL_AHB1_GRP1_PERIPH_CRC},
    /*FuriHalBusTSC*/ {Bus::AHB1, BusGroup::GRP1, LL_AHB1_GRP1_PERIPH_TSC},

    /*FuriHalBusGPIOA*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_GPIOA},
    /*FuriHalBusGPIOB*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_GPIOB},
    /*FuriHalBusGPIOC*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_GPIOC},
    /*FuriHalBusGPIOD*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_GPIOD},
    /*FuriHalBusGPIOE*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_GPIOE},
    /*FuriHalBusGPIOH*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_GPIOH},
#if defined(ADC_SUPPORT_5_MSPS)
    /*FuriHalBusADC*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_ADC},
#endif
    /*FuriHalBusAES1*/ {Bus::AHB2, BusGroup::GRP1, LL_AHB2_GRP1_PERIPH_AES1},

    /*FuriHalBusQUADSPI*/ {Bus::AHB3, BusGroup::GRP1, LL_AHB3_GRP1_PERIPH_QUADSPI},
    /*FuriHalBusPKA*/ {Bus::AHB3, BusGroup::GRP1, LL_AHB3_GRP1_PERIPH_PKA},
    /*FuriHalBusAES2*/ {Bus::AHB3, BusGroup::GRP1, LL_AHB3_GRP1_PERIPH_AES2},
    /*FuriHalBusRNG*/ {Bus::AHB3, BusGroup::GRP1, LL_AHB3_GRP1_PERIPH_RNG},
    /*FuriHalBusHSEM*/ {Bus::AHB3, BusGroup::GRP1, LL_AHB3_GRP1_PERIPH_HSEM},
    /*FuriHalBusIPCC*/ {Bus::AHB3, BusGroup::GRP1, LL_AHB3_GRP1_PERIPH_IPCC},
    /*FuriHalBusFLASH*/ {Bus::AHB3, BusGroup::GRP1, LL_AHB3_GRP1_PERIPH_FLASH},

    /*FuriHalBusTIM2*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_TIM2},
    /*FuriHalBusLCD*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_LCD},
    /*FuriHalBusSPI2*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_SPI2},
    /*FuriHalBusI2C1*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_I2C1},
    /*FuriHalBusI2C3*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_I2C3},
    /*FuriHalBusCRS*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_CRS},
    /*FuriHalBusUSB*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_USB},
    /*FuriHalBusLPTIM1*/ {Bus::APB1, BusGroup::GRP1, LL_APB1_GRP1_PERIPH_LPTIM1},

    /*FuriHalBusLPUART1*/ {Bus::APB1, BusGroup::GRP2, LL_APB1_GRP2_PERIPH_LPUART1},
    /*FuriHalBusLPTIM2*/ {Bus::APB1, BusGroup::GRP2, LL_APB1_GRP2_PERIPH_LPTIM2},

#if defined(ADC_SUPPORT_2_5_MSPS)
    /*FuriHalBusADC*/ {Bus::APB2, BusGroup::GRP1, LL_APB2_GRP1_PERIPH_ADC},
#endif
    /*FuriHalBusTIM1*/ {Bus::APB2, BusGroup::GRP1, LL_APB2_GRP1_PERIPH_TIM1},
    /*FuriHalBusSPI1*/ {Bus::APB2, BusGroup::GRP1, LL_APB2_GRP1_PERIPH_SPI1},
    /*FuriHalBusUSART1*/ {Bus::APB2, BusGroup::GRP1, LL_APB2_GRP1_PERIPH_USART1},
    /*FuriHalBusTIM16*/ {Bus::APB2, BusGroup::GRP1, LL_APB2_GRP1_PERIPH_TIM16},
    /*FuriHalBusTIM17*/ {Bus::APB2, BusGroup::GRP1, LL_APB2_GRP1_PERIPH_TIM17},
    /*FuriHalBusSAI1*/ {Bus::APB2, BusGroup::GRP1, LL_APB2_GRP1_PERIPH_SAI1},

    /*FuriHalBusRF*/ {Bus::APB3, BusGroup::GRP1, LL_APB3_GRP1_PERIPH_RF},
};

static_assert(std::size(bus_table) == FuriHalBusMAX, "FuriHalBus enum and bus_table size mismatch");

inline void furi_hal_bus_clock_enable(Bus bus, uint32_t value, BusGroup grp) {
    switch(bus) {
    case Bus::AHB1:
        LL_AHB1_GRP1_EnableClock(value);
        break;
    case Bus::AHB2:
        LL_AHB2_GRP1_EnableClock(value);
        break;
    case Bus::AHB3:
        LL_AHB3_GRP1_EnableClock(value);
        break;
    case Bus::APB1:
        if(grp == BusGroup::GRP1)
            LL_APB1_GRP1_EnableClock(value);
        else
            LL_APB1_GRP2_EnableClock(value);
        break;
    case Bus::APB2:
        LL_APB2_GRP1_EnableClock(value);
        break;
    case Bus::APB3:
        // APB3: clock control not available
        break;
    }
}

inline void furi_hal_bus_reset_assert(Bus bus, uint32_t value, BusGroup grp) {
    switch(bus) {
    case Bus::AHB1:
        LL_AHB1_GRP1_ForceReset(value);
        break;
    case Bus::AHB2:
        LL_AHB2_GRP1_ForceReset(value);
        break;
    case Bus::AHB3:
        LL_AHB3_GRP1_ForceReset(value);
        break;
    case Bus::APB1:
        if(grp == BusGroup::GRP1) {
            LL_APB1_GRP1_ForceReset(value);
        } else {
            LL_APB1_GRP2_ForceReset(value);
        }
        break;
    case Bus::APB2:
        LL_APB2_GRP1_ForceReset(value);
        break;
    case Bus::APB3:
        LL_APB3_GRP1_ForceReset(value);
        break;
    }
}

inline void furi_hal_bus_reset_deassert(Bus bus, uint32_t value, BusGroup grp) {
    switch(bus) {
    case Bus::AHB1:
        LL_AHB1_GRP1_ReleaseReset(value);
        break;
    case Bus::AHB2:
        LL_AHB2_GRP1_ReleaseReset(value);
        break;
    case Bus::AHB3:
        LL_AHB3_GRP1_ReleaseReset(value);
        break;
    case Bus::APB1:
        if(grp == BusGroup::GRP1)
            LL_APB1_GRP1_ReleaseReset(value);
        else
            LL_APB1_GRP2_ReleaseReset(value);
        break;
    case Bus::APB2:
        LL_APB2_GRP1_ReleaseReset(value);
        break;
    case Bus::APB3:
        LL_APB3_GRP1_ReleaseReset(value);
        break;
    }
}

inline void furi_hal_bus_clock_disable(Bus bus, uint32_t value, BusGroup grp) {
    switch(bus) {
    case Bus::AHB1:
        LL_AHB1_GRP1_DisableClock(value);
        break;
    case Bus::AHB2:
        LL_AHB2_GRP1_DisableClock(value);
        break;
    case Bus::AHB3:
        LL_AHB3_GRP1_DisableClock(value);
        break;
    case Bus::APB1:
        if(grp == BusGroup::GRP1)
            LL_APB1_GRP1_DisableClock(value);
        else
            LL_APB1_GRP2_DisableClock(value);
        break;
    case Bus::APB2:
        LL_APB2_GRP1_DisableClock(value);
        break;
    case Bus::APB3:
        // APB3: clock control not available
        break;
    }
}

inline bool
    furi_hal_bus_is_reset_deasserted(Bus bus, uint32_t value, BusGroup grp = BusGroup::GRP1) {
    switch(bus) {
    case Bus::AHB1:
        return (READ_BIT(RCC->AHB1RSTR, value) == 0);
    case Bus::AHB2:
        return (READ_BIT(RCC->AHB2RSTR, value) == 0);
    case Bus::AHB3:
        return (READ_BIT(RCC->AHB3RSTR, value) == 0);
    case Bus::APB1:
        if(grp == BusGroup::GRP1)
            return (READ_BIT(RCC->APB1RSTR1, value) == 0);
        else
            return (READ_BIT(RCC->APB1RSTR2, value) == 0);
    case Bus::APB2:
        return (READ_BIT(RCC->APB2RSTR, value) == 0);
    case Bus::APB3:
        return (READ_BIT(RCC->APB3RSTR, value) == 0);
    }
    return false;
}

inline bool furi_hal_bus_is_clock_enabled(Bus bus, uint32_t value, BusGroup grp = BusGroup::GRP1) {
    switch(bus) {
    case Bus::AHB1:
        return (READ_BIT(RCC->AHB1ENR, value) == value);
    case Bus::AHB2:
        return (READ_BIT(RCC->AHB2ENR, value) == value);
    case Bus::AHB3:
        return (READ_BIT(RCC->AHB3ENR, value) == value);
    case Bus::APB1:
        if(grp == BusGroup::GRP1)
            return (READ_BIT(RCC->APB1ENR1, value) == value);
        else
            return (READ_BIT(RCC->APB1ENR2, value) == value);
    case Bus::APB2:
        return (READ_BIT(RCC->APB2ENR, value) == value);
    case Bus::APB3:
        // Clock control not available for APB3
        return true;
    }
    return false;
}

inline bool
    furi_hal_bus_is_periph_enabled(Bus bus, uint32_t value, BusGroup grp = BusGroup::GRP1) {
    return furi_hal_bus_is_reset_deasserted(bus, value, grp) &&
           furi_hal_bus_is_clock_enabled(bus, value, grp);
}

inline void furi_hal_bus_periph_enable(Bus bus, uint32_t value, BusGroup grp) {
    furi_hal_bus_clock_enable(bus, value, grp);
    furi_hal_bus_reset_deassert(bus, value, grp);
}

inline void furi_hal_bus_periph_disable(Bus bus, uint32_t value, BusGroup grp) {
    furi_hal_bus_reset_assert(bus, value, grp);
    furi_hal_bus_clock_disable(bus, value, grp);
}

void furi_hal_bus_init_early() {
    FURI_CRITICAL_ENTER();

    furi_hal_bus_periph_disable(Bus::APB1, FURI_HAL_BUS_APB1_GRP1, BusGroup::GRP1);
    furi_hal_bus_periph_disable(Bus::APB1, FURI_HAL_BUS_APB1_GRP2, BusGroup::GRP2);
    furi_hal_bus_periph_disable(Bus::APB2, FURI_HAL_BUS_APB2_GRP1, BusGroup::GRP1);
    furi_hal_bus_periph_disable(Bus::APB3, FURI_HAL_BUS_APB3_GRP1, BusGroup::GRP1);

    FURI_CRITICAL_EXIT();
}
void furi_hal_bus_deinit_early(void) {
    FURI_CRITICAL_ENTER();

    furi_hal_bus_periph_enable(Bus::APB1, FURI_HAL_BUS_APB1_GRP1, BusGroup::GRP1);
    furi_hal_bus_periph_enable(Bus::APB1, FURI_HAL_BUS_APB1_GRP2, BusGroup::GRP2);
    furi_hal_bus_periph_enable(Bus::APB2, FURI_HAL_BUS_APB2_GRP1, BusGroup::GRP1);
    furi_hal_bus_periph_enable(Bus::APB3, FURI_HAL_BUS_APB3_GRP1, BusGroup::GRP1);

    FURI_CRITICAL_EXIT();
}

void furi_hal_bus_enable(FuriHalBus bus) {
    furi_check(bus < FuriHalBusMAX);

    const BusInfo& info = bus_table[bus];

    // Пропускаем, если не инициализировано (0 = IGNORE)
    if(info.periph == 0) return;

    FURI_CRITICAL_ENTER();
    furi_hal_bus_periph_enable(info.bus, info.periph, info.group);
    FURI_CRITICAL_EXIT();
}

void furi_hal_bus_reset(FuriHalBus bus) {
    furi_check(bus < FuriHalBusMAX);

    const BusInfo& info = bus_table[bus];
    if(!info.periph) return;

    FURI_CRITICAL_ENTER();
    furi_hal_bus_reset_assert(info.bus, info.periph, info.group);
    furi_hal_bus_reset_deassert(info.bus, info.periph, info.group);
    FURI_CRITICAL_EXIT();
}

void furi_hal_bus_disable(FuriHalBus bus) {
    furi_check(bus < FuriHalBusMAX);

    const BusInfo& info = bus_table[bus];
    if(info.periph == 0) {
        return;
    }

    FURI_CRITICAL_ENTER();

    furi_check(furi_hal_bus_is_periph_enabled(info.bus, info.periph, info.group));
    furi_hal_bus_periph_disable(info.bus, info.periph, info.group);

    FURI_CRITICAL_EXIT();
}

bool furi_hal_bus_is_enabled(FuriHalBus bus) {
    furi_check(bus < FuriHalBusMAX);

    const BusInfo& info = bus_table[bus];

    // Пропускаем, если IGNORE
    if(info.periph == 0) return true;

    FURI_CRITICAL_ENTER();
    bool ret = furi_hal_bus_is_periph_enabled(info.bus, info.periph, info.group);
    FURI_CRITICAL_EXIT();

    return ret;
}

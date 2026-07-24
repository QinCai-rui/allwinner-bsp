/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Compatibility header for linux/of_gpio.h removed in kernel 6.13+
 * Provides the old OF GPIO API using the modern gpiod API.
 */

#ifndef _LINUX_OF_GPIO_H
#define _LINUX_OF_GPIO_H

#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/fwnode.h>

enum of_gpio_flags {
	OF_GPIO_ACTIVE_LOW = 0x1,
	OF_GPIO_SINGLE_ENDED = 0x2,
	OF_GPIO_OPEN_DRAIN = 0x4,
	OF_GPIO_TRANSITORY = 0x8,
	OF_GPIO_PULL_UP = 0x10,
	OF_GPIO_PULL_DOWN = 0x20,
	OF_GPIO_PULL_DISABLE = 0x40,
};

/**
 * of_get_named_gpiod_flags - Get a GPIO descriptor and flags
 * @np:		device node to get GPIO from
 * @propname:	property name containing gpio specifier(s)
 * @index:	index of the GPIO
 * @flags:	a flags pointer to fill in
 *
 * Returns:
 * GPIO descriptor to use with Linux GPIO API, or ERR_PTR() on error.
 * If @flags is not NULL the function also fills in flags for the GPIO.
 */
static inline struct gpio_desc *
of_get_named_gpiod_flags(const struct device_node *np,
			 const char *propname, int index,
			 enum of_gpio_flags *flags)
{
	struct gpio_desc *desc;
	enum gpiod_flags dflags = GPIOD_ASIS;

	desc = fwnode_gpiod_get_index(of_fwnode_handle(np), propname,
				      index, dflags, NULL);
	if (IS_ERR(desc))
		return desc;

	if (flags) {
		*flags = 0;
		if (gpiod_is_active_low(desc))
			*flags |= OF_GPIO_ACTIVE_LOW;
	}

	return desc;
}

/**
 * of_get_named_gpio_flags - Get a GPIO number and flags
 * @np:		device node to get GPIO from
 * @propname:	property name containing gpio specifier(s)
 * @index:	index of the GPIO
 * @flags:	a flags pointer to fill in
 *
 * Returns:
 * GPIO number to use with legacy GPIO API, or negative errno on error.
 * If @flags is not NULL the function also fills in flags for the GPIO.
 */
static inline int
of_get_named_gpio_flags(const struct device_node *np,
			const char *propname, int index,
			enum of_gpio_flags *flags)
{
	struct gpio_desc *desc;

	desc = of_get_named_gpiod_flags(np, propname, index, flags);
	if (IS_ERR(desc))
		return PTR_ERR(desc);

	return desc_to_gpio(desc);
}

/**
 * of_get_named_gpio - Get a GPIO number
 * @np:		device node to get GPIO from
 * @propname:	property name containing gpio specifier(s)
 * @index:	index of the GPIO
 *
 * Returns:
 * GPIO number to use with legacy GPIO API, or negative errno on error.
 */
static inline int
of_get_named_gpio(const struct device_node *np, const char *propname, int index)
{
	return of_get_named_gpio_flags(np, propname, index, NULL);
}

/**
 * of_get_gpio_flags - Get a GPIO number and flags by index
 * @np:		device node to get GPIO from
 * @index:	index of the GPIO
 * @flags:	a flags pointer to fill in
 *
 * Returns:
 * GPIO number to use with legacy GPIO API, or negative errno on error.
 * If @flags is not NULL the function also fills in flags for the GPIO.
 */
static inline int
of_get_gpio_flags(const struct device_node *np, int index,
		  enum of_gpio_flags *flags)
{
	return of_get_named_gpio_flags(np, "gpios", index, flags);
}

/**
 * of_get_gpio - Get a GPIO number by index
 * @np:		device node to get GPIO from
 * @index:	index of the GPIO
 *
 * Returns:
 * GPIO number to use with legacy GPIO API, or negative errno on error.
 */
static inline int
of_get_gpio(const struct device_node *np, int index)
{
	return of_get_gpio_flags(np, index, NULL);
}

#endif /* _LINUX_OF_GPIO_H */
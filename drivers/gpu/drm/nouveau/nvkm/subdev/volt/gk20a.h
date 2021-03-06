/*
 * Copyright (c) 2015, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __GK20A_VOLT_H__
#define __GK20A_VOLT_H__

#define MAX_THERMAL_LIMITS 8

struct cvb_coef {
	int c0;
	int c1;
	int c2;
	int c3;
	int c4;
	int c5;
};

struct gk20a_volt_priv {
	struct nvkm_volt base;
	struct regulator *vdd;
	const int *thermal_table;
	u8 therm_nr;
	u8 therm_idx;
	struct mutex therm_lock;
	struct thermal_cooling_device *cdev;
	struct nvkm_voltage scale_table[MAX_THERMAL_LIMITS][MAX_DVFS_FREQS];
};

int gk20a_volt_calc_voltage(struct gk20a_volt_priv *priv,
		const struct cvb_coef *coef, int speedo, int therm_idx);
int gk20a_volt_round_voltage(struct gk20a_volt_priv *priv, int uv);
int gk20a_volt_vid_get(struct nvkm_volt *volt);
int gk20a_volt_vid_set(struct nvkm_volt *volt, u8 vid);
int gk20a_volt_set_id(struct nvkm_volt *volt, u8 id, int condition);

#ifdef CONFIG_THERMAL
int gk20a_volt_dvfs_cdev_register(struct gk20a_volt_priv *priv);
void gk20a_volt_dvfs_cdev_unregister(struct gk20a_volt_priv *priv);
#else
static inline int gk20a_volt_dvfs_cdev_register(struct gk20a_volt_priv *priv)
{
	return -EINVAL;
}
static inline void
gk20a_volt_dvfs_cdev_unregister(struct gk20a_volt_priv *priv) {}
#endif

#endif

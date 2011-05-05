#ifndef FREESCALE_MMA7260_H_
#define FREESCALE_MMA7260_H_

typedef struct {
	// znormalizowana wartość napięcia [-850,850] odpowiada [-1g,1g]
	int x_normal_mv;
	int y_normal_mv;
	int z_normal_mv;

	// wartość napięcia odczytana z akcelerometru
	unsigned int x_mv;
	unsigned int y_mv;
	unsigned int z_mv;
} MMA7260_OUTPUT;

void MMA7260_ReadOutput(unsigned int channel_x, unsigned int channel_y,
		unsigned int channel_z, void(*callback)(MMA7260_OUTPUT));

#endif /* FREESCALE_MMA7260_H_ */
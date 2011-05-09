/*
 * pedometer.c
 *
 *  Created on: Feb 18, 2011
 *      Author: lhanusiak
 */

#include <rtt/rtt.h>
#include <adc/adc.h>
#include <math.h>

#include <peripherals/adc_helper.h>

#include "modules/freescale_mma7260.h"
#include "modules/cd4053b.h"
#include "modules/l3g4200d.h"

#include "pedometer.h"

extern Twid twid;

volatile PEDOMETER_CONFIG def_config;
volatile MMA7260_OUTPUT mma_output;
volatile unsigned int pedometer_enabled = 0;

void (*onStepCallback)(void) = NULL;

unsigned int detected_steps = 0;
unsigned int positive_peak_timestamp = 0;
unsigned int positive_acc_peak = 0;
unsigned int negative_peak_timestamp = 0;
unsigned int negative_acc_peak = 0;

void init_pedometer_config()
{
  def_config.negative_thld = 750;
  def_config.positive_thld = 850;

  def_config.sample_time = 4;
}

unsigned int get_step_count()
{
  return detected_steps / 2;
}

void acc_data_ready(MMA7260_OUTPUT mma7260_output)
{
  mma_output = mma7260_output;
}

void run_step_detector(MMA7260_OUTPUT mma7260_output)
{
  TRACE_DEBUG("-- Step detector (MMA7260) --\n\r");
  unsigned int peak_type = 0;
  int output = sqrt(pow(mma7260_output.x_normal_mv, 2) + pow(
      mma7260_output.y_normal_mv, 2) + pow(mma7260_output.z_normal_mv, 2));
  //	int output = ABS(mma7260_output.z_normal_mv);

  if (output <= def_config.negative_thld)
  {
    negative_acc_peak = output;
    negative_peak_timestamp = RTT_GetTime(AT91C_BASE_RTTC);
    peak_type = NEGATIVE_PEAK;
    TRACE_DEBUG("-- Negative peak detected: %dmV, limit: %dmV, time: %d --\n\r",
        output, def_config.negative_thld, negative_peak_timestamp);
  }

  if (output >= def_config.positive_thld)
  {
    positive_acc_peak = output;
    positive_peak_timestamp = RTT_GetTime(AT91C_BASE_RTTC);
    peak_type = POSITIVE_PEAK;
    TRACE_DEBUG("-- Positive peak detected: %dmV, limit: %dmV, time: %d --\n\r",
        output, def_config.positive_thld, positive_peak_timestamp);
  }

  if (peak_type && negative_peak_timestamp && positive_peak_timestamp)
  {
    int sample_duration = ABS(negative_peak_timestamp
        - positive_peak_timestamp);
    TRACE_DEBUG("-- Sample duration: %d --\n\r", sample_duration);

    if (sample_duration <= def_config.sample_time)
    {
      TRACE_INFO("-- Step detected. Total steps: %d --\n\r", (detected_steps + 1) / 2);
      detected_steps++;
      negative_peak_timestamp = 0;
      positive_peak_timestamp = 0;
      if (!(detected_steps % 2))
      {
        onStepCallback();
        //				L3G4200D_Reset(&twid);
      }
    }
    else
    {
      if (peak_type == POSITIVE_PEAK)
      {
        TRACE_DEBUG("-- Reset negative sample --\n\r");
        negative_peak_timestamp = 0;
      }
      else
      {
        TRACE_DEBUG("-- Reset positive sample --\n\r");
        positive_peak_timestamp = 0;
      }
    }
  }
}

void start_steps_detection(void(*callback)(void))
{
  TRACE_DEBUG("-- Step detector init --\n\r");

  detected_steps = 0;
  pedometer_enabled = 1;
  positive_peak_timestamp = 0;
  negative_peak_timestamp = 0;
  onStepCallback = callback;

  CD4053_EnableAccelerometer();

  TRACE_DEBUG("-- RTT Timer init --\n\r");
  RTT_SetPrescaler(AT91C_BASE_RTTC, 32768 / 2);

  while (pedometer_enabled)
  {
    L3G4200D_ReadData(&twid);

    if (ADC_IsDataReady())
    {
      run_step_detector(mma_output);
    }
    MMA7260_ReadOutput(ADC_CHANNEL_5, ADC_CHANNEL_6, ADC_CHANNEL_7,
        acc_data_ready);
  }
}

unsigned int stop_counting_steps()
{
  pedometer_enabled = 0;
  TRACE_DEBUG("-- Step detector disabled. Total steps: %d --\n\r", detected_steps / 2);

  return detected_steps / 2;
}

#include <stdio.h>
#include "includes/button.h"
#include "includes/sampleA2D.h"
#include "includes/joystick.h"
#include "includes/sampleAnalyzer.h"
#include "includes/interface.h"

int main(void)
{
	int length = 0;

	samplerDatapoint_t *current_sample;
	printf("Starting up...\n");

	Interface_Start();

	while (!Interface_isCancelled())
	{
		Sampler_startSampling();
		sleep(1); // sample for 1 second
		Sampler_stopSampling();

		current_sample = Sampler_extractAllValues(&length);

		Analyzer_Start(current_sample, length);
		Analyzer_Close();
	}
	Analyzer_Cancel();
	Interface_Close();

	printf("Done shutdown! Goodbye!\n");
	return 0;
}

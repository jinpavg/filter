/// @file
/// @copyright	Copyright (c) 2017, Cycling '74
/// @author		Timothy Place
/// @license	Usage of this file and its contents is governed by the MIT License

#include "DspFilters/Legendre.h"
using namespace Dsp::Legendre::Design;
#include "../filter.h"


class legendre : public filter<legendre,1,false,false> {
public:

	MIN_DESCRIPTION { "Nth-order elliptic lowpass filter" };
	MIN_TAGS		{ "filters" };
	MIN_AUTHOR		{ "Cycling '74" };
	MIN_RELATED		{ "filterdesign, filterdetail, slide, filter.elliptic" };

	inlet<>		in	{ this, "(number) input to be filtered" };
	outlet<>	out	{ this, "(number) filtered output" };

	legendre(const atoms& args = {})
	: filter(args)
	{}


	attribute<double> m_samplerate { this, "samplerate", 44100.0,
		description { "Sampling frequency of the filter." },
	};


	double samplerate() override {
		return m_samplerate;
	}


	message<> number { this, "number", "Stream of numbers to be filtered.",
		MIN_FUNCTION {
			double x[1] { args[0] };
			double* y = x;

			// we use the pending filter because this object is threadsafe using a copy
			if (m_update_pending) {
				m_filter = std::move(m_filter_pending);
				m_update_pending = false;
			}
			m_filter->process(1, &y);


			out.send(*y);
			return {};
		}
	};
};


MIN_EXTERNAL(legendre);

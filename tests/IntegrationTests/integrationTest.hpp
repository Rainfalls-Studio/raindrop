#ifndef __INTEGRATION_TEST_HPP__
#define __INTEGRATION_TEST_HPP__

#include <Raindrop/Raindrop.hpp>
#include <gtest/gtest.h>
#include <test.hpp>

class ApplicationTest : public ::testing::Test{
	public:
		ApplicationTest() = default;
		virtual ~ApplicationTest() = default;

		static void SetUpTestCase(){
			
		}

		static Raindrop::Application application;
};

#endif
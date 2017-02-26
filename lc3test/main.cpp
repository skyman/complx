#include <lc3_all.hpp>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <wx/tokenzr.h>
#include <wx/xml/xml.h>
#include <wx/wx.h>

#include <XmlTestParser.hpp>

struct suite_summary
{
    suite_summary() : grade(0), tests(0), executions(0), warnings(0) {}
    int grade;
    int tests;
    int executions;
    int warnings;
};

unsigned int digits(unsigned int v)
{
    return 1 + ((v >= 1000000000) ? 9 : (v >= 100000000) ? 8 : (v >= 10000000) ? 7 :
                (v >= 1000000) ? 6 : (v >= 100000) ? 5 : (v >= 10000) ? 4 :
                (v >= 1000) ? 3 : (v >= 100) ? 2 : (v >= 10) ? 1 : 0);
}

int main(int argc, char** argv)
{
    // Typical
    if (argc < 3)
    {
        printf("Usage: lc3test testfile.xml asmfile.asm -random_seed=int -runs=int\n");
        return EXIT_FAILURE;
    }

    lc3_test_suite suite;
    std::string xmlfile = argv[1];
    std::string asmfile = argv[2];
    int random_seed = -1;
    unsigned int runs = 1;

    for (int i = 3; i < argc; i++)
    {
        const std::string& arg = argv[i];
        if (arg.find("-random_seed=") != std::string::npos)
            random_seed = atoi(arg.substr(arg.find("=") + 1).c_str());
        else if (arg.find("-runs=") != std::string::npos)
            runs = (unsigned int) atoi(arg.substr(arg.find("=") + 1).c_str());
        else
        {
            printf("Invalid param %s passed in!", arg.c_str());
            return EXIT_FAILURE;
        }
    }

    try
    {
        if (!XmlTestParser().LoadTestSuite(suite, xmlfile))
        {
            printf("Xml file not found or parse errors found\n");
            return EXIT_FAILURE;
        }
    }
    catch (XmlTestParserException x)
    {
        printf("%s\n", x.what().c_str());
        return EXIT_FAILURE;
    }

    std::vector<suite_summary> results;
    int num_passed = 0;

    lc3_test_suite test_suite;
    for (unsigned int i = 0; i < runs; i++)
    {
        test_suite = suite;
        try
        {
            lc3_run_test_suite(test_suite, asmfile, random_seed);
        }
        catch (const char* x)
        {
            printf("%s\n", x);
            return EXIT_FAILURE;
        }
        catch (std::string x)
        {
            printf("%s\n", x.c_str());
            return EXIT_FAILURE;
        }


        std::stringstream oss;
        lc3_write_test_report(oss, test_suite, asmfile);
        if (runs > 1)
            std::cout << "Run " << (i + 1) << ": ";
        std::cout << oss.str() << "\n";

        if (random_seed != -1) random_seed++;
        if (test_suite.passed) num_passed++;

        suite_summary summary;
        summary.grade = test_suite.points;
        for (const auto& test : test_suite.tests)
        {
            if (test.passed) summary.tests++;
            summary.warnings += test.warnings;
            summary.executions += test.executions;
        }
        results.push_back(summary);
    }

    if (runs > 1)
    {
        std::cout << "Overall results\n";
        std::cout << "----------------\n\n";
        for (unsigned int i = 0; i < results.size(); i++)
        {
            const auto& summary = results[i];
            std::cout << "Run " << std::setw(digits(runs)) << (i+1) << " Grade: " << std::setw(3) << summary.grade << "/" << test_suite.max_points << " Tests Passed: " << summary.tests << "/" << test_suite.tests.size() << " Executions: " << summary.executions << " Warnings: " << summary.warnings << "\n";
        }
    }


    return EXIT_SUCCESS;
}

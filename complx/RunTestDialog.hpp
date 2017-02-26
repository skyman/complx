#ifndef RUN_TEST_DIALOG_HPP
#define RUN_TEST_DIALOG_HPP

#include "RunTestDialogDecl.h"
#include <lc3_all.hpp>
#include <string>

class TestSuiteInfoPanel;
class TestCaseInfoPanel;
class CheckInfoPanel;

class TestTreeItem : public wxTreeItemData
{
public:
    TestTreeItem(void* data);
    void* data;
};

class RunTestDialog : public RunTestDialogDecl
{
public:
    RunTestDialog(wxWindow* parent, const std::string& filename, lc3_test_suite& test);
    void UpdateTests();
protected:
    void OnSelectTestItem(wxTreeEvent& event);
    void OnRunTest(wxCommandEvent& event);
    void OnRunTests(wxCommandEvent& event);
    void OnReport(wxCommandEvent& event);
    void OnTestReport(wxCommandEvent& event);
    lc3_test_suite& suite;
    std::string filename;
private:
    TestSuiteInfoPanel* tspanel;
    TestCaseInfoPanel* tcpanel;
    CheckInfoPanel* cipanel;
};

class TestSuiteInfoPanel : public TestSuiteInfoPanelDecl
{
public:
    TestSuiteInfoPanel(wxWindow* parent, lc3_test_suite& suite);
    void Update();
private:
    lc3_test_suite& suite;
};

class TestCaseInfoPanel : public TestCaseInfoPanelDecl
{
public:
    TestCaseInfoPanel(wxWindow* parent);
    void Update();
    void Update(lc3_test& testcase);
    void OnWarnings(wxCommandEvent& event);
private:
    lc3_test* testcase;
};

class CheckInfoPanel : public CheckInfoPanelDecl
{
public:
    CheckInfoPanel(wxWindow* parent);
    void Update();
    void Update(lc3_test_output& output);
private:
    lc3_test_output* output;
};

class TestReportDialog : public TestReportDialogDecl
{
public:
    TestReportDialog(wxWindow* parent, const wxString& name, const wxString& report);
};

#endif

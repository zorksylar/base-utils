#include <stdio.h>

#include "unittest.h"
#include "logging.h"
#include "strop.h"

namespace base {

void TestCase::fail() {
    failures_++;
}

TestMgr* TestMgr::instance_s = nullptr;

TestMgr* TestMgr::instance() {
    if (instance_s == nullptr) {
        instance_s = new TestMgr;
    }
    return instance_s;
}

TestCase* TestMgr::reg(TestCase* t) {
    tests_.push_back(t);
    return t;
}

void TestMgr::matched_tests(const char* match, std::vector<TestCase*>* matched) {
    // TODO
}

int TestMgr::parse_args(int argc, char* argv[], int* n_proc, bool* show_help, std::vector<TestCase*>* selected) {
    *show_help = false;
    *n_proc = 1;
    char* select = nullptr;
    char* skip = nullptr;
    std::vector<TestCase*> match;
    for (int i = 1; i < argc; i++) {
        if (streq(argv[i], "-h") || streq(argv[i], "--help")) {
            *show_help = true;
        } else if (streq(argv[i], "-p")) {
            if (i == argc - 1 || (*n_proc = atoi(argv[i + 1])) <= 0) {
                return 1;
            }
        } else if (startswith(argv[i], "--select=")) {
            select = argv[i] + strlen("--select=");
            matched_tests(select, &match);
        } else if (startswith(argv[i], "--skip=")) {
            skip = argv[i] + strlen("--skip=");
            matched_tests(skip, &match);
        } else {
            return 1;
        }
    }
    if (select == nullptr && skip == nullptr) {
        *selected = tests_;
    } else if (select != nullptr && skip == nullptr) {
        selected->clear();
    } else if (select == nullptr && skip != nullptr) {
        *selected = tests_;
    } else { // select != nullptr && skip != nullptr
        printf("please provide either --select or --skip, not both\n");
        return 1;
    }
    return 0;
}

int TestMgr::run(int argc, char* argv[]) {
    bool show_help;
    int n_proc;
    std::vector<TestCase*> selected;
    int r = parse_args(argc, argv, &n_proc, &show_help, &selected);
    if (r != 0 || show_help) {
        printf("usage: %s [-h|--help] [-p n_proc] [--select=a/b,c/d|--skip=a/b,c/d]\n", argv[0]);
        return r;
    }

    int failures = 0;
    int passed = 0;
    if (selected.size() > 0) {
        Log::info("The following %d test cases will be checked:", selected.size());
        for (auto t : selected) {
            Log::info("    %s/%s", t->group(), t->name());
        }
    }
    for (auto t : selected) {
        Log::info("--> starting test: %s/%s", t->group(), t->name());
        // TODO run tests in parallel, and in subprocesses
        t->run();
        failures += t->failures();
        if (t->failures() == 0) {
            Log::info("<-- passed test: %s/%s", t->group(), t->name());
            passed++;
        } else {
            Log::error("X-- failed test: %s/%s", t->group(), t->name());
        }
    }
    Log::info("%d/%lu passed, %d failures\n", passed, selected.size(), failures);
    return failures;
}

} // namespace base

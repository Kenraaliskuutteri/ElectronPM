#include <ncurses.h>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <array>

static std::string run_capture(const std::string& cmd) {
    std::array<char, 4096> buf{};
    std::string out;
    FILE* p = popen(cmd.c_str(), "r");
    if (!p) return "Failed to run command\n";
    while (fgets(buf.data(), (int)buf.size(), p)) out += buf.data();
    pclose(p);
    return out;
}

static void write_log(const std::string& path, const std::string& s) {
    std::ofstream f(path);
    f << s;
}

static std::string prolog_hint(const std::string& text) {
    std::string cmd = "swipl -q -s tools/epm_tester/fuzzy.pl -g main -t halt -- " + text;
    return run_capture(cmd);
}

static std::string r_hint(const std::string& logfile) {
    std::string cmd = "Rscript tools/epm_tester/smart.R " + logfile;
    return run_capture(cmd);
}

static std::string compile_one(const std::string& file) {
    std::string cmd = "gcc -std=c11 -Wall -Wextra -O2 -Iinclude -c " + file + " -o /tmp/epm_test.o 2>&1";
    return run_capture(cmd);
}

static std::string api_link_check() {
    std::string cmd =
    "bash -lc 'cat > /tmp/epm_api_check.c <<EOF\n"
    "#include <epm/backend.h>\n"
    "int main(){ epm_ctx_t c; return epm_init(&c, \".epmdb\", 0); }\n"
    "EOF\n"
    "gcc -std=c11 -Wall -Wextra -O2 -Iinclude /tmp/epm_api_check.c "
    "src/backend/backend.c src/backend/install.c src/backend/remove.c src/backend/list.c src/backend/db.c src/backend/atom.c "
    "src/common/fs.c src/common/util.c src/common/logging.c -o /tmp/epm_api_check 2>&1'";
    return run_capture(cmd);
}

static std::string build_all() {
    return run_capture("make clean >/dev/null 2>&1; make 2>&1");
}

static std::string sandbox_smoke() {
    std::string cmd =
    "bash -lc 'set -e;"
    "export EPM_TEST_MODE=1;"
    "export EPM_DB_ROOT=./.epmdb_test;"
    "rm -rf ./.epmdb_test ./_sandbox_pkg ./_sandbox_root smoke.atom;"
    "mkdir -p ./_sandbox_pkg/payload/usr/local/bin;"
    "cat > ./_sandbox_pkg/epm.json <<EOF\n"
    "{\n"
    "  \"name\": \"smoke\",\n"
    "  \"version\": \"1.0.0\",\n"
    "  \"prefix\": \"./_sandbox_root\",\n"
    "  \"description\": \"tester smoke\"\n"
    "}\n"
    "EOF\n"
    "cat > ./_sandbox_pkg/payload/usr/local/bin/smoke <<EOF\n"
    "#!/bin/sh\n"
    "echo smoke-ok\n"
    "EOF\n"
    "chmod +x ./_sandbox_pkg/payload/usr/local/bin/smoke;"
    "tar -C ./_sandbox_pkg -cf smoke.atom epm.json payload;"
    "mkdir -p ./_sandbox_root;"
    "./electronpm install smoke.atom;"
    "./electronpm list;"
    "./electronpm remove smoke;"
    "echo DONE'";
    return run_capture(cmd);
}

static void boxed(WINDOW* w, const char* title) {
    box(w, 0, 0);
    mvwprintw(w, 0, 2, " %s ", title);
    wrefresh(w);
}

static void render_text(WINDOW* w, const std::string& text, const char* title) {
    werase(w);
    boxed(w, title);
    int maxy, maxx;
    getmaxyx(w, maxy, maxx);
    std::istringstream iss(text);
    std::string line;
    int y = 1;
    while (std::getline(iss, line) && y < maxy - 1) {
        if ((int)line.size() > maxx - 2) line = line.substr(0, maxx - 5) + "...";
        mvwprintw(w, y++, 1, "%s", line.c_str());
    }
    wrefresh(w);
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int H, W;
    getmaxyx(stdscr, H, W);

    int leftW = W / 3;
    WINDOW* left = newwin(H, leftW, 0, 0);
    WINDOW* right = newwin(H, W - leftW, 0, leftW);

    std::vector<std::string> menu = {
        "Compile backend.c",
        "Compile install.c",
        "Compile remove.c",
        "Compile list.c",
        "Compile db.c",
        "Compile atom.c",
        "Compile fs.c",
        "Compile util.c",
        "Compile logging.c",
        "API link check",
        "Build project (make)",
        "Sandbox smoke test",
        "Quit"
    };

    std::vector<std::string> files = {
        "src/backend/backend.c",
        "src/backend/install.c",
        "src/backend/remove.c",
        "src/backend/list.c",
        "src/backend/db.c",
        "src/backend/atom.c",
        "src/common/fs.c",
        "src/common/util.c",
        "src/common/logging.c"
    };

    int sel = 0;

    for (;;) {
        werase(left);
        boxed(left, "EPM Tester  |  ↑↓ Enter  |  q");
        for (int i = 0; i < (int)menu.size(); i++) {
            if (i == sel) wattron(left, A_REVERSE);
            mvwprintw(left, i + 1, 1, "%s", menu[i].c_str());
            if (i == sel) wattroff(left, A_REVERSE);
        }
        wrefresh(left);

        int ch = wgetch(left);
        if (ch == 'q') break;
        if (ch == KEY_UP) sel = (sel - 1 + (int)menu.size()) % (int)menu.size();
        if (ch == KEY_DOWN) sel = (sel + 1) % (int)menu.size();

        if (ch == '\n') {
            std::string result;

            if (sel >= 0 && sel <= 8) {
                result = compile_one(files[sel]);
                if (result.empty()) result = "OK (compiled clean)\n";
            } else if (menu[sel] == "API link check") {
                result = api_link_check();
                if (result.empty()) result = "OK (API links)\n";
            } else if (menu[sel] == "Build project (make)") {
                result = build_all();
            } else if (menu[sel] == "Sandbox smoke test") {
                result = sandbox_smoke();
            } else if (menu[sel] == "Quit") {
                break;
            }

            std::string logfile = "/tmp/epm_tester.log";
            write_log(logfile, result);

            std::string ph = prolog_hint(result);
            std::string rh = r_hint(logfile);

            std::string panel = result + "\n\nProlog Hint:\n" + ph + "\nR Insight:\n" + rh;
            render_text(right, panel, "Output + Diagnosis");
        }
    }

    delwin(left);
    delwin(right);
    endwin();
    return 0;
}

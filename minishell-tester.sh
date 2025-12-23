#!/bin/bash
# ==========================================================
# Minishell STRICT AST-Compatible Test Suite
# No semicolons
# No fd-dup (2>&1)
# FD redirections only: 0< 1> 2>
# ==========================================================

MINISHELL="${1:-./minishell}"

BASH_OUT="bash_out.txt"
MINI_OUT="mini_out.txt"
BASH_EXIT="bash_exit.txt"
MINI_EXIT="mini_exit.txt"

PASS=0
FAIL=0
COUNT=0

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[1;33m"
BLUE="\033[0;34m"
NC="\033[0m"

setup() {
    rm -f *.txt
    echo "hello world" > in.txt
    echo "error msg" > err.txt
    mkdir -p dir
    echo "file1" > dir/a.txt
    echo "file2" > dir/b.txt
    export TESTVAR="value"
}

cleanup() {
    rm -rf *.txt dir
}

run_test() {
    COUNT=$((COUNT + 1))
    local name="$1"
    local cmd="$2"

    echo -e "\n${BLUE}Test $COUNT:${NC} ${YELLOW}$name${NC}"
    echo "$cmd"

    bash -c "$cmd" >"$BASH_OUT" 2>"$BASH_EXIT"
    echo $? >>"$BASH_EXIT"

    echo "$cmd" | "$MINISHELL" >"$MINI_OUT" 2>"$MINI_EXIT"
    echo $? >>"$MINI_EXIT"

    if diff -q "$BASH_OUT" "$MINI_OUT" >/dev/null; then
        echo -e "${GREEN}✓ PASS${NC}"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "--- bash ---"
        cat "$BASH_OUT"
        echo "--- minishell ---"
        cat "$MINI_OUT"
        FAIL=$((FAIL + 1))
    fi
}

setup

# ==========================================================
# BASIC COMMANDS
# ==========================================================
run_test "echo" "echo hello"
run_test "echo quotes" "echo \"hello world\""
run_test "echo single quotes" "echo 'hello world'"
run_test "var expand" "echo \$TESTVAR"

# ==========================================================
# REDIRECTIONS (AST COMPATIBLE)
# ==========================================================
run_test "stdout redirect" "echo hello 1>out.txt"
run_test "stdout no space" "echo hello 1>out.txt"
run_test "stdin redirect" "cat 0<in.txt"
run_test "stderr redirect" "ls nonexistent 2>err.txt"
run_test "append redirect" "echo hi >> out.txt"

# ==========================================================
# PIPES
# ==========================================================
run_test "simple pipe" "echo hello | cat"
run_test "pipe chain" "echo hello | cat | cat"
run_test "pipe + redirect" "echo hello | cat 1>out.txt"

# ==========================================================
# LOGICAL OPERATORS
# ==========================================================
run_test "AND success" "echo ok && echo yes"
run_test "AND fail" "ls nope && echo yes"
run_test "OR success" "echo ok || echo fail"
run_test "OR fail" "ls nope || echo recovered"
run_test "AND OR combo" "echo ok && ls nope || echo fallback"

# ==========================================================
# SUBSHELLS
# ==========================================================
run_test "simple subshell" "(echo hello)"
run_test "subshell pipe" "(echo hello) | cat"
run_test "nested subshell" "((echo hi))"
run_test "subshell logical" "(echo ok && echo yes)"

# ==========================================================
# HEREDOC
# ==========================================================
run_test "simple heredoc" "cat << EOF
hello
world
EOF"

run_test "heredoc with pipe" "cat << EOF | cat
hello heredoc
EOF"

run_test "heredoc logical" "cat << EOF && echo done
hi
EOF"

run_test "heredoc subshell" "(cat << EOF)
subshell
EOF"

# ==========================================================
# COMBINED STRESS
# ==========================================================
run_test "redir + pipe + and" "echo hi 1>out.txt && cat out.txt | cat"
run_test "subshell + pipe + or" "(ls nope) | cat || echo recovered"
run_test "fd stdin + subshell" "(cat) 0<in.txt"

cleanup

echo -e "\n${BLUE}==============================${NC}"
echo -e "${GREEN}PASSED:${NC} $PASS"
echo -e "${RED}FAILED:${NC} $FAIL"
echo -e "${BLUE}==============================${NC}"

exit $FAIL


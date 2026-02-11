diagnose(Text, Hint) :-
    downcase_atom(Text, T),
    (   sub_atom(T, _, _, _, "database locked") ->
        Hint = "Lock contention: another process is holding the EPM lock. Close other runs or remove stale lock file."
    ;   sub_atom(T, _, _, _, "permission denied") ->
        Hint = "Permission issue: run as root or set EPM_TEST_MODE=1 for sandbox runs."
    ;   sub_atom(T, _, _, _, "conflict") ->
        Hint = "Conflict: file already exists or is owned by another package. Use EPM_OVERWRITE=1 only if you accept risk."
    ;   sub_atom(T, _, _, _, "invalid .atom package") ->
        Hint = "Manifest/format issue: ensure .atom contains epm.json and payload/. Check required keys name/version."
    ;   sub_atom(T, _, _, _, "undefined reference") ->
        Hint = "Linker failure: missing source file in makefile SRC list or missing function implementation."
    ;   sub_atom(T, _, _, _, "implicit declaration") ->
        Hint = "Prototype missing: header not included or function not declared before use."
    ;   sub_atom(T, _, _, _, "conflicting types") ->
        Hint = "Signature mismatch: header declaration differs from implementation."
    ;   sub_atom(T, _, _, _, "no such file or directory") ->
        Hint = "Missing include path or file. Verify -Iinclude and file locations."
    ;   sub_atom(T, _, _, _, "segmentation fault") ->
        Hint = "Runtime crash: likely NULL deref, buffer overflow, or use-after-free. Find first failing callsite."
    ;   Hint = "Unclear. Focus on the first error line in the log; later errors are often noise."
    ).

main :-
    current_prolog_flag(argv, Argv),
    atomic_list_concat(Argv, " ", Text),
    diagnose(Text, Hint),
    writeln(Hint).


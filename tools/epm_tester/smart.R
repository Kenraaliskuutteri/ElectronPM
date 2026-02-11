args <- commandArgs(trailingOnly=TRUE)
if (length(args) < 1) quit(status=2)

log <- paste(readLines(args[1], warn=FALSE), collapse="\n")
lines <- strsplit(log, "\n")[[1]]

first_err_idx <- grep("error:", lines, fixed=TRUE)
first_err <- if (length(first_err_idx) > 0) lines[first_err_idx[1]] else "(no compiler error line found)"

score <- function(p) if (grepl(p, log, ignore.case=TRUE)) 1 else 0

cats <- c(
  "Link/Makefile integration" = score("undefined reference"),
  "Include/path issue" = score("no such file or directory"),
  "Signature mismatch" = score("conflicting types"),
  "Missing prototype" = score("implicit declaration"),
  "Duplicate symbol" = score("redefinition"),
  "Permission/runtime env" = score("permission denied|database locked|conflict")
)

best <- names(which.max(cats))
cat("R-Insight: ", best, "\n", sep="")
cat("First error: ", first_err, "\n", sep="")

from pathlib import Path
import difflib
import sys
import subprocess
from subprocess import STDOUT, PIPE


test_path = Path(sys.argv[1])
test_module = sys.argv[2]
program_name = sys.argv[3]

test_name = test_path.name
ans_path = (test_path.parent / (f"{test_module}_{test_name}")).with_suffix(".ans")
log_path = Path(f"{test_module}_{test_name}").with_suffix(".log")

FAILED_MSG = f"- test {test_name} failed. see {log_path} for details"
SUCCESS_MSG = f"+ test {test_name} successful"

return_code = 1

with (
    test_path.open() as test_data,
    open(ans_path) as ans_data
):
    res = subprocess.run(program_name, stdin=test_data, stdout=PIPE,
                         stderr=STDOUT, text=True)

    if res.returncode != 0:
        return_code = 1
        print(FAILED_MSG)
        with open(log_path, 'w') as log:
            log.writelines(res.stdout)
    else:
        reslines = res.stdout.splitlines(True)
        anslines = ans_data.readlines()
        
        if reslines == anslines:
            return_code = 0
            print(SUCCESS_MSG)
        else:
            return_code = 1
            print(FAILED_MSG)
            diff = difflib.Differ().compare(anslines, reslines)
            with open(log_path, 'w') as log:
                log.write("Diff:\n")
                log.writelines(diff)

exit(return_code)

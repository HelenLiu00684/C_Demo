import subprocess
import pytest

def run_unit_test(unit_test_name):
    result = subprocess.run(
        ["./" + unit_test_name],
        capture_output=True,
        text=True
    )
    print(result.stdout)
    assert result.returncode == 0

def test_unit():
    run_unit_test("C_tests/test_transaction")    
    run_unit_test("C_tests/test_symbolmap")    
    run_unit_test("C_tests/test_inventory")    
    run_unit_test("C_tests/test_account")      
    run_unit_test("C_tests/test_customer")        

def test_stress():
    run_unit_test("python3_test/test_transaction_append_stress")    
             
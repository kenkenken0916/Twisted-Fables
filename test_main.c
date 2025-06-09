#include <stdio.h>
#include "test_system.h"
#include "debug_log.h"

int main(void) {
    printf("=== 扭曲寓言測試系統 ===\n\n");
    
    TestResult result = run_all_tests();
    
    if (result.failed > 0) {
        printf("\n測試未全部通過，請查看 test.log 檔案了解詳情。\n");
        return 1;
    }
    
    printf("\n所有測試通過！\n");
    return 0;
}

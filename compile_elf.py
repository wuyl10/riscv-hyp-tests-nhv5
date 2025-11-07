#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
RISC-V 单独编译测试脚本 (Python版本)
用法：python3 compile_single.py [start_line] [end_line]
     python3 compile_single.py all
"""

import os
import sys
import re
import subprocess
import shutil
from pathlib import Path

# 配置
SOURCE_FILE = "test_register.c"
BACKUP_FILE = "test_register.c.backup"
OUTPUT_DIR = "individual_tests"

def print_usage():
    """打印使用说明"""
    print("RISC-V 单独编译测试脚本 (Python版本)")
    print("=" * 40)
    print("用法:")
    print(f"  {sys.argv[0]} 250 300     # 编译第250-300行的测试")
    print(f"  {sys.argv[0]} all         # 编译所有测试")
    print()
    print("当前test_register.c中的活动TEST_REGISTER行:")
    
    try:
        with open(SOURCE_FILE, 'r', encoding='utf-8') as f:
            lines = f.readlines()
        
        count = 0
        for i, line in enumerate(lines, 1):
            if re.match(r'^\s*TEST_REGISTER\(', line):
                print(f"{i}:{line.strip()}")
                count += 1
                if count >= 10:
                    print("...")
                    break
    except FileNotFoundError:
        print(f"错误: 找不到文件 {SOURCE_FILE}")

def extract_test_lines(start_line=None, end_line=None):
    """提取测试行"""
    test_lines = []
    
    try:
        with open(SOURCE_FILE, 'r', encoding='utf-8') as f:
            lines = f.readlines()
    except FileNotFoundError:
        print(f"错误: 找不到文件 {SOURCE_FILE}")
        return []
    
    if start_line is None:  # all模式
        for i, line in enumerate(lines, 1):
            if re.match(r'^\s*TEST_REGISTER\(', line):
                test_lines.append((i, line.strip()))
    else:
        for i in range(start_line, end_line + 1):
            if i <= len(lines):
                line = lines[i - 1]
                if 'TEST_REGISTER(' in line:
                    test_lines.append((i, line.strip()))
    
    return test_lines

def extract_test_name(line_content):
    """从TEST_REGISTER行提取测试名称"""
    match = re.search(r'TEST_REGISTER\(([^)]*)\)', line_content)
    if match:
        return match.group(1)
    return None

def backup_and_modify_source(line_num):
    """备份并修改源文件"""
    # 备份原文件
    shutil.copy2(SOURCE_FILE, BACKUP_FILE)
    
    # 读取文件内容
    with open(SOURCE_FILE, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # 注释所有TEST_REGISTER行
    for i, line in enumerate(lines):
        if re.match(r'^\s*TEST_REGISTER\(', line):
            lines[i] = '// ' + line
    
    # 取消注释当前行
    if line_num <= len(lines):
        lines[line_num - 1] = re.sub(r'^\s*//\s*TEST_REGISTER\(', 'TEST_REGISTER(', lines[line_num - 1])
    
    # 写回文件
    with open(SOURCE_FILE, 'w', encoding='utf-8') as f:
        f.writelines(lines)

def restore_source():
    """恢复源文件"""
    if os.path.exists(BACKUP_FILE):
        shutil.copy2(BACKUP_FILE, SOURCE_FILE)

def run_make(first_build=False):
    """执行make命令"""
    try:
        # 只在第一次编译前执行clean
        if first_build:
            subprocess.run(['make', 'clean'], 
                         stdout=subprocess.DEVNULL, 
                         stderr=subprocess.DEVNULL, 
                         check=False)
        
        # 执行编译
        result = subprocess.run(['make', 'PLAT=nemu', 'LOG_LEVEL=4', 'CROSS_COMPILE=riscv64-unknown-elf-'], 
                              stdout=subprocess.DEVNULL, 
                              stderr=subprocess.DEVNULL)
        return result.returncode == 0
    except Exception as e:
        print(f"编译错误: {e}")
        return False

def copy_output_files(test_name):
    """复制输出文件"""
    output_path = Path(OUTPUT_DIR)
    
    # 复制ELF文件（改为大写扩展名）
    elf_src = Path('build/nemu/rvh_test.elf')
    if elf_src.exists():
        elf_dst = output_path / f"{test_name}.ELF"
        try:
            shutil.copy2(elf_src, elf_dst)
        except Exception:
            pass
    
    # 复制ASM文件
    asm_src = Path('build/nemu/rvh_test.asm')
    if asm_src.exists():
        asm_dst = output_path / f"{test_name}.asm"
        try:
            shutil.copy2(asm_src, asm_dst)
        except Exception:
            pass

def main():
    """主函数"""
    # 检查参数
    if len(sys.argv) < 2:
        print_usage()
        return 1
    
    # 创建输出目录
    Path(OUTPUT_DIR).mkdir(exist_ok=True)
    
    # 解析参数
    if sys.argv[1] == 'all':
        test_lines = extract_test_lines()
    else:
        try:
            start_line = int(sys.argv[1])
            end_line = int(sys.argv[2]) if len(sys.argv) > 2 else start_line
            test_lines = extract_test_lines(start_line, end_line)
        except (ValueError, IndexError):
            print("错误: 参数格式不正确")
            print_usage()
            return 1
    
    if not test_lines:
        print("没有找到任何测试")
        return 1
    
    print("找到以下测试:")
    for line_num, content in test_lines:
        print(f"{line_num}:{content}")
    print()
    
    # 编译统计
    success_count = 0
    fail_count = 0
    first_build = True
    
    # 编译每个测试
    for line_num, content in test_lines:
        test_name = extract_test_name(content)
        if not test_name:
            continue
        
        print(f"编译: {test_name}")
        
        # 修改源文件
        backup_and_modify_source(line_num)
        
        # 编译
        if run_make(first_build):
            print("  ✓ 成功")
            copy_output_files(test_name)
            success_count += 1
        else:
            print("  ✗ 失败")
            fail_count += 1
        
        first_build = False
    
    # 恢复原文件
    restore_source()
    
    print()
    print(f"编译完成: 成功 {success_count}, 失败 {fail_count}")
    print(f"输出文件在: {OUTPUT_DIR}/")
    
    return 0

if __name__ == '__main__':
    sys.exit(main())

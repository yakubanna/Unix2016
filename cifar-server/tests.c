#include "stringbuilder.h"
#include "stringutils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void TestQueryString() {
    assert(GetIntParam("", "res") == 0);
    assert(GetIntParam("res=1", "res") == 1);
    assert(GetIntParam("result=1", "res") == 0);
    assert(GetIntParam("a=10&b=20&c=30", "a") == 10);
    assert(GetIntParam("a=10&b=20&c=30", "b") == 20);
    assert(GetIntParam("a=10&b=20&c=30", "c") == 30);
    assert(GetIntParam("a=10&b=20&c=30", "d") == 0);
}

static void TestStringBuilder1() {
    struct TStringBuilder sb;
    TStringBuilder_Init(&sb);
    TStringBuilder_AppendCStr(&sb, "hello");
    TStringBuilder_AppendCStr(&sb, " ");
    TStringBuilder_AppendCStr(&sb, "world!");
    TStringBuilder_AppendCStr(&sb, "ahdjahskjdhjsakdhss");
    TStringBuilder_AppendBuf(&sb, "sS", 1);
    assert(strcmp(sb.Data, "hello world!ahdjahskjdhjsakdhsss") == 0);

    TStringBuilder_Clear(&sb);
    assert(strcmp(sb.Data, "") == 0);

    TStringBuilder_Sprintf(&sb, "Hello world! %d %d", 1000, 32);
    assert(strcmp(sb.Data, "Hello world! 1000 32") == 0);

    TStringBuilder_Destroy(&sb);
}

static void TestStringBuilder2() {
    struct TStringBuilder sb;
    TStringBuilder_Init(&sb);
    for (int i = 0; i < 100000; ++i) {
        TStringBuilder_Sprintf(&sb, "%d", 7);
    }
    assert(strlen(sb.Data) == 100000);
    assert(sb.Length == 100000);
    TStringBuilder_Destroy(&sb);
}

static void TestStartsWith() {
    assert(StartsWith("", ""));
    assert(!StartsWith("", "a"));
    assert(StartsWith("a", "a"));
    assert(StartsWith("aaa", "aa"));
    assert(!StartsWith("aaa", "ab"));
}

static void TestEndsWith() {
    assert(EndsWithCI("", ""));
    assert(!EndsWithCI("", "a"));
    assert(EndsWithCI("a", "a"));
    assert(EndsWithCI("aaa", "aa"));
    assert(EndsWithCI("aaa", "AA"));
    assert(EndsWithCI("image.jpg", ".JPG"));
    assert(!EndsWithCI("image.jpg", ".JPEG"));
    assert(!EndsWithCI("aaa", "ab"));
}

int main(void) {
    TestQueryString();
    TestStringBuilder1();
    TestStringBuilder2();
    TestStartsWith();
    TestEndsWith();
    printf("TESTS PASSED\n");
    return 0;
}

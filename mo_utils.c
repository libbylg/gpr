#include "mo_utils.h"


static mo_cm mo_cms[256] =
{
/*  0x00    ???    0    */  0,
/*  0x01    ???    1    */  0,
/*  0x02    ???    2    */  0,
/*  0x03    ???    3    */  0,
/*  0x04    ???    4    */  0,
/*  0x05    ???    5    */  0,
/*  0x06    ???    6    */  0,
/*  0x07    ???    7    */  0,
/*  0x08    ???    8    */  0,
/*  0x09    HT     9    */  MO_CM_SPACE,
/*  0x0A    ???   10    */  MO_CM_NEWLINE | MO_CM_STRING_FLAG | MO_CM_COMMENT_FLAG,
/*  0x0B    VT    11    */  MO_CM_SPACE,
/*  0x0C    ???   12    */  0,
/*  0x0D    ???   13    */  0,
/*  0x0E    ???   14    */  0,
/*  0x0F    ???   15    */  0,
/*  0x10    ???   16    */  0,
/*  0x11    ???   17    */  0,
/*  0x12    ???   18    */  0,
/*  0x13    ???   19    */  0,
/*  0x14    ???   20    */  0,
/*  0x15    ???   21    */  0,
/*  0x16    ???   22    */  0,
/*  0x17    ???   23    */  0,
/*  0x18    ???   24    */  0,
/*  0x19    ???   25    */  0,
/*  0x1A    ???   26    */  0,
/*  0x1B    ???   27    */  0,
/*  0x1C    ???   28    */  0,
/*  0x1D    ???   29    */  0,
/*  0x1E    ???   30    */  0,
/*  0x1F    ???   31    */  0,
/*  0x20          32    */  0,
/*  0x21    !     33    */  0,
/*  0x22    "     34    */  MO_CM_STRING_FLAG,
/*  0x23    #     35    */  0,
/*  0x24    $     36    */  0,
/*  0x25    %     37    */  0,
/*  0x26    &     38    */  0,
/*  0x27    '     39    */  0,
/*  0x28    (     40    */  0,
/*  0x29    )     41    */  0,
/*  0x2A    *     42    */  MO_CM_COMMENT_FLAG,
/*  0x2B    +     43    */  0,
/*  0x2C    ,     44    */  0,
/*  0x2D    -     45    */  0,
/*  0x2E    .     46    */  0,
/*  0x2F    /     47    */  0,
/*  0x30    0     48    */  MO_CM_DEC,
/*  0x31    1     49    */  MO_CM_DEC,
/*  0x32    2     50    */  MO_CM_DEC,
/*  0x33    3     51    */  MO_CM_DEC,
/*  0x34    4     52    */  MO_CM_DEC,
/*  0x35    5     53    */  MO_CM_DEC,
/*  0x36    6     54    */  MO_CM_DEC,
/*  0x37    7     55    */  MO_CM_DEC,
/*  0x38    8     56    */  MO_CM_DEC,
/*  0x39    9     57    */  MO_CM_DEC,
/*  0x3A    :     58    */  0,
/*  0x3B    ;     59    */  0,
/*  0x3C    <     60    */  0,
/*  0x3D    =     61    */  0,
/*  0x3E    >     62    */  0,
/*  0x3F    ?     63    */  0,
/*  0x40    @     64    */  0,
/*  0x41    A     65    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x42    B     66    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x43    C     67    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x44    D     68    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x45    E     69    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x46    F     70    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x47    G     71    */  MO_CM_ALPHA,
/*  0x48    H     72    */  MO_CM_ALPHA,
/*  0x49    I     73    */  MO_CM_ALPHA,
/*  0x4A    J     74    */  MO_CM_ALPHA,
/*  0x4B    K     75    */  MO_CM_ALPHA,
/*  0x4C    L     76    */  MO_CM_ALPHA,
/*  0x4D    M     77    */  MO_CM_ALPHA,
/*  0x4E    N     78    */  MO_CM_ALPHA,
/*  0x4F    O     79    */  MO_CM_ALPHA,
/*  0x50    P     80    */  MO_CM_ALPHA,
/*  0x51    Q     81    */  MO_CM_ALPHA,
/*  0x52    R     82    */  MO_CM_ALPHA,
/*  0x53    S     83    */  MO_CM_ALPHA,
/*  0x54    T     84    */  MO_CM_ALPHA,
/*  0x55    U     85    */  MO_CM_ALPHA,
/*  0x56    V     86    */  MO_CM_ALPHA,
/*  0x57    W     87    */  MO_CM_ALPHA,
/*  0x58    X     88    */  MO_CM_ALPHA,
/*  0x59    Y     89    */  MO_CM_ALPHA,
/*  0x5A    Z     90    */  MO_CM_ALPHA,
/*  0x5B    [     91    */  0,
/*  0x5C    \     92    */  MO_CM_STRING_FLAG,
/*  0x5D    ]     93    */  0,
/*  0x5E    ^     94    */  0,
/*  0x5F    _     95    */  MO_CM_ALPHA | MO_CM_UDRLINE,
/*  0x60    `     96    */  0,
/*  0x61    a     97    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x62    b     98    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x63    c     99    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x64    d    100    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x65    e    101    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x66    f    102    */  MO_CM_ALPHA | MO_CM_HEX,
/*  0x67    g    103    */  MO_CM_ALPHA,
/*  0x68    h    104    */  MO_CM_ALPHA,
/*  0x69    i    105    */  MO_CM_ALPHA,
/*  0x6A    j    106    */  MO_CM_ALPHA,
/*  0x6B    k    107    */  MO_CM_ALPHA,
/*  0x6C    l    108    */  MO_CM_ALPHA,
/*  0x6D    m    109    */  MO_CM_ALPHA,
/*  0x6E    n    110    */  MO_CM_ALPHA,
/*  0x6F    o    111    */  MO_CM_ALPHA,
/*  0x70    p    112    */  MO_CM_ALPHA,
/*  0x71    q    113    */  MO_CM_ALPHA,
/*  0x72    r    114    */  MO_CM_ALPHA,
/*  0x73    s    115    */  MO_CM_ALPHA,
/*  0x74    t    116    */  MO_CM_ALPHA,
/*  0x75    u    117    */  MO_CM_ALPHA,
/*  0x76    v    118    */  MO_CM_ALPHA,
/*  0x77    w    119    */  MO_CM_ALPHA,
/*  0x78    x    120    */  MO_CM_ALPHA,
/*  0x79    y    121    */  MO_CM_ALPHA,
/*  0x7A    z    122    */  MO_CM_ALPHA,
/*  0x7B    {    123    */  0,
/*  0x7C    |    124    */  0,
/*  0x7D    }    125    */  0,
/*  0x7E    ~    126    */  0,
/*  0x7F         127    */  0,
/*  0x80         128    */  0,
/*  0x81         129    */  0,
/*  0x82         130    */  0,
/*  0x83         131    */  0,
/*  0x84         132    */  0,
/*  0x85         133    */  0,
/*  0x86         134    */  0,
/*  0x87         135    */  0,
/*  0x88         136    */  0,
/*  0x89         137    */  0,
/*  0x8A         138    */  0,
/*  0x8B         139    */  0,
/*  0x8C         140    */  0,
/*  0x8D         141    */  0,
/*  0x8E         142    */  0,
/*  0x8F         143    */  0,
/*  0x90         144    */  0,
/*  0x91         145    */  0,
/*  0x92         146    */  0,
/*  0x93         147    */  0,
/*  0x94         148    */  0,
/*  0x95         149    */  0,
/*  0x96         150    */  0,
/*  0x97         151    */  0,
/*  0x98         152    */  0,
/*  0x99         153    */  0,
/*  0x9A         154    */  0,
/*  0x9B         155    */  0,
/*  0x9C         156    */  0,
/*  0x9D         157    */  0,
/*  0x9E         158    */  0,
/*  0x9F         159    */  0,
/*  0xA0         160    */  0,
/*  0xA1         161    */  0,
/*  0xA2         162    */  0,
/*  0xA3         163    */  0,
/*  0xA4         164    */  0,
/*  0xA5         165    */  0,
/*  0xA6         166    */  0,
/*  0xA7         167    */  0,
/*  0xA8         168    */  0,
/*  0xA9         169    */  0,
/*  0xAA         170    */  0,
/*  0xAB         171    */  0,
/*  0xAC         172    */  0,
/*  0xAD         173    */  0,
/*  0xAE         174    */  0,
/*  0xAF         175    */  0,
/*  0xB0         176    */  0,
/*  0xB1         177    */  0,
/*  0xB2         178    */  0,
/*  0xB3         179    */  0,
/*  0xB4         180    */  0,
/*  0xB5         181    */  0,
/*  0xB6         182    */  0,
/*  0xB7         183    */  0,
/*  0xB8         184    */  0,
/*  0xB9         185    */  0,
/*  0xBA         186    */  0,
/*  0xBB         187    */  0,
/*  0xBC         188    */  0,
/*  0xBD         189    */  0,
/*  0xBE         190    */  0,
/*  0xBF         191    */  0,
/*  0xC0         192    */  0,
/*  0xC1         193    */  0,
/*  0xC2         194    */  0,
/*  0xC3         195    */  0,
/*  0xC4         196    */  0,
/*  0xC5         197    */  0,
/*  0xC6         198    */  0,
/*  0xC7         199    */  0,
/*  0xC8         200    */  0,
/*  0xC9         201    */  0,
/*  0xCA         202    */  0,
/*  0xCB         203    */  0,
/*  0xCC         204    */  0,
/*  0xCD         205    */  0,
/*  0xCE         206    */  0,
/*  0xCF         207    */  0,
/*  0xD0         208    */  0,
/*  0xD1         209    */  0,
/*  0xD2         210    */  0,
/*  0xD3         211    */  0,
/*  0xD4         212    */  0,
/*  0xD5         213    */  0,
/*  0xD6         214    */  0,
/*  0xD7         215    */  0,
/*  0xD8         216    */  0,
/*  0xD9         217    */  0,
/*  0xDA         218    */  0,
/*  0xDB         219    */  0,
/*  0xDC         220    */  0,
/*  0xDD         221    */  0,
/*  0xDE         222    */  0,
/*  0xDF         223    */  0,
/*  0xE0         224    */  0,
/*  0xE1         225    */  0,
/*  0xE2         226    */  0,
/*  0xE3         227    */  0,
/*  0xE4         228    */  0,
/*  0xE5         229    */  0,
/*  0xE6         230    */  0,
/*  0xE7         231    */  0,
/*  0xE8         232    */  0,
/*  0xE9         233    */  0,
/*  0xEA         234    */  0,
/*  0xEB         235    */  0,
/*  0xEC         236    */  0,
/*  0xED         237    */  0,
/*  0xEE         238    */  0,
/*  0xEF         239    */  0,
/*  0xF0         240    */  0,
/*  0xF1         241    */  0,
/*  0xF2         242    */  0,
/*  0xF3         243    */  0,
/*  0xF4         244    */  0,
/*  0xF5         245    */  0,
/*  0xF6         246    */  0,
/*  0xF7         247    */  0,
/*  0xF8         248    */  0,
/*  0xF9         249    */  0,
/*  0xFA         250    */  0,
/*  0xFB         251    */  0,
/*  0xFC         252    */  0,
/*  0xFD         253    */  0,
/*  0xFE         254    */  0,
/*  0xFF         255    */  0,
};


MO_EXTERN mo_byte* mo_lex_skipspace(struct lex_t*  x, struct result_t* r, mo_byte* pc)
{
    ///!    跳过所有的空白
    x->pos = pc;
    while (mo_cms[(int)*pc] & MO_CM_SPACE) {
        pc++;
    }

    x->pos = pc;
    return x->pos;
}

MO_EXTERN mo_byte* mo_lex_skipcomment(struct lex_t*  x, struct result_t* r, mo_byte* pc)
{
    //  如果是单行注释
    if ((pc[0] == '/') && (pc[1] == '/')) {
        return mo_lex_skipline(x, r, pc);
    }

    if ((pc[0] == '/') && (pc[1] == '*')) {
        pc += 2;

        while (mo_result_ok(r)) {
            while (!(mo_cms[*pc] & (MO_CM_COMMENT_FLAG))) {
                pc++;
            }

            if ((pc[0] == '*') && (pc[1] == '/')) {
                x->pos = pc + 2;
                return x->pos;
            }

            if ((x->end - pc) < 2) {
                pc = mo_lex_loadmore(x, r, pc);
                if ((x->end - pc) < 2) {
                    mo_result_errorf(r, 111, "Comment not end with '*/'");
                    return x->pos;
                }
            }
        }
    }

    mo_result_errorf(r, 111, "Comment not begin with '/*' and '//'");
    return x->pos;
}

MO_EXTERN mo_byte* mo_lex_loadmore(struct lex_t* x, struct result_t* r, mo_byte* pc)
{
    struct lex_t* cache = x;

    //  先保存下pos的位置
    x->pos = pc;

    //  如果缓冲区太过于靠近缓冲区的尾部，那么将数据移动一次
    if ((x->limit - x->pos) < MO_TOKEN_SIZE_MAX) {
        int offset = x->pos - x->buf;
        int len = x->end - x->pos;
        if (0 > len) {
            memmove(x->buf, x->pos, len);
        }
        if (offset > 0) {
            x->pos = x->buf;
            x->end = x->pos + len;
            x->end[0] = '\n';
            x->anchor->line -= offset;
        }
    }

RETRY:
    //  从输入流再读取部分数据
    int ret = x->stream->read(x->stream->ctx, &(x->end), x->limit);
    if (MO_READ_OK == ret) {
        return x->pos;
    }

    if (MO_READ_EOF == ret) {
        struct stream_t* old_stream = x->stream;
        x->stream = x->stream->prev;
        old_stream->close(old_stream->ctx);
        if (NULL == x->stream) {
            return x->pos;
        }
        goto RETRY;
    }

    mo_result_errorf(r, 111, "read from stream failed");
    return x->pos;
}

//  定位到第一个非注释非空白处
MO_EXTERN mo_byte* mo_lex_locate(struct lex_t* x, struct result_t* r, mo_byte* pc)
{
    register struct lex_t*    cache;

RETRY:
    cache = x;
    pc = x->pos;

    //  跳过所有的空白
    pc = mo_lex_skipspace(x, r, pc);
    
    //  如果预留的数据区太少，那么重新载入数据
    if ((x->end - pc) < MO_TOKEN_SIZE_MAX) {
        pc = mo_lex_loadmore(x, r, pc);
    }

    //  如果遇到换行,需要判断到底是遇到真换行还是遇到的是缓冲区的数据被读取完了
    if (*pc != '\n') {
        x->pos = pc; ///<    先将已经忽略过的空白跳过
        return MO_READ_OK;
    }

    //  如果确实遇到了文本换行
    if (pc != x->end) {
        mo_lex_newline(x, r, pc);
        goto RETRY;
    }

    //  从缓冲区读取更多数据
    pc = mo_lex_loadmore(x, r, pc);

    //  如果读取失败，那么返回
    if (!mo_lex_ok(x)) {
        return x->pos;
    }

    //  如果读取成功，但是数据并没有增加，说明到了文件尾，直接返回即可
    if (x->pos == x->end) {
        return x->pos;
    }

    //  如果读取数据成功，且数据增加，那么继续尝试一次
    goto RETRY;
}


MO_EXTERN   mo_byte*            mo_lex_newline(struct lex_t* x, struct result_t* r, mo_byte* pc)
{
    x->anchor->lino++;  ///<    进入下一行
    x->anchor->line = pc + 1;
    x->pos = pc + 1;
    return x->pos;
}

MO_EXTERN   mo_byte*            mo_lex_skipline(struct lex_t* x, struct result_t* r, mo_byte* pc)
{
    while (mo_result_ok(r)) {
        if (*pc == '\n') {
            //  如果pc处的换行并不是真换行，而是缓冲区数据结束标记，那么尝试载入更多数据
            if (pc == x->end) {
                pc = mo_lex_loadmore(x, r, pc);
                //  重新加载数据后，指针位置没有变化，说明没有更多数据了
                if (pc == x->end) {
                    x->pos = pc;
                    return x->pos;
                }

                //  如果已经载入了更多的数据，那么继续当前的识别操作
                continue;
            }

            //  这里说明遇到了真换行，此时标记一下换行标记
            return mo_lex_newline(x, r, pc);
        }

        pc++;
    }
}

static mo_byte*            mo_lex_accept_number_hex(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{
    x->pos = pc + 2; //  跳过0x前缀
    while (mo_cms[*pc] & (MO_CM_HEX)) {
        pc++;
    }

    if (mo_cms[*pc] & (MO_CM_ALPHA)) {
        mo_result_errorf(r, 111, "");
    }
    
    x->pos = pc;
    return x->pos;
}

static mo_byte*            mo_lex_accept_number_dec(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{
    x->pos = pc + 1; //  跳过已经识别过的数字前缀
    while (mo_cms[*pc] & (MO_CM_DEC)) {
        pc++;
    }

    //  如果遇到浮点数关键字符
    if (('.' == *pc) || ('e' == *pc) || ('E' == *pc)) {
        return mo_lex_accept_number_float_postfix(x, k, r, pc);
    }

    if (mo_cms[*pc] & (MO_CM_ALPHA)) {
        mo_result_errorf(r, 111, "");
    }
    
    x->pos = pc;
    return x->pos;
}

static mo_byte*            mo_lex_accept_number_oct(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{
    x->pos = pc + 1; //  跳过八进制的 0 前缀
    while (mo_cms[*pc] & (MO_CM_OCT)) {
        pc++;
    }

    if (mo_cms[*pc] & (MO_CM_ALPHA | MO_CM_DEC)) {
        mo_result_errorf(r, 111, "");
    }
    
    x->pos = pc;
    return x->pos;
}

static mo_byte*            mo_lex_accept_number_float_postfix(struct lex_t* x, struct token_t* k, struct result_t* r, mo_byte* pc)
{
    //  如果是小数部分
    if ('.' == *pc) {
        pc++;
        while (mo_cms[*pc] & (MO_CM_DEC)) {
            pc++;
        }
    }

    //  如果是指数部分
    if (('e' == *pc) || ('E' == *pc)) {
        pc++;
        if (('+' == *pc) || ('-' == *pc)) {
            pc++;
        }
        while (mo_cms[*pc] & (MO_CM_DEC)) {
            pc++;
        }
    }

    //  浮点数后面如果是字母结束，是错误的，比如：12.car
    if (mo_cms[*pc] & (MO_CM_ALPHA)) {
        mo_result_errorf(r, 111, "");
        return x->pos;
    }

    //  浮点数识别结束
    x->pos = pc;
    return x->pos;
}

MO_EXTERN struct token_t*     mo_lex_accept_number(struct lex_t* x, struct token_t* k, struct result_t* r)
{
    mo_byte* pc = x->pos;
    if ('0' == *pc) {
        //  如果遇到十六进制前缀
        if (('x' == pc[1]) || ('X' == pc[1])) {
            return mo_lex_accept_number_hex(x, k, r, pc);
        }

        //  如果遇到的是8进制前缀
        if (mo_cms[pc[1]] & (MO_CM_OCT)) {
            return mo_lex_accept_number_oct(x, k, r, pc);
        }

        //  如果遇到的上浮点识别符
        if (('.' == pc[1]) || ('e' == pc[1]) || ('E' == pc[1])) {
            return mo_lex_accept_number_float_postfix(x, k, r, pc);
        }

        //  如果遇到了字母，这种情况算作浮点数格式错误
        if (mo_cms[pc[1]] & (MO_CM_ALPHA)) {
            mo_result_errorf(r, 111, "");
            return pc;
        }

        //  全部排除后可以作为十进制数来识别：这种情况其实就只有个0
        return mo_lex_accept_number_dec(x, k, r, pc);
    }

    //  如果起始字符就是十进制数字，那么当作十进制数字来识别
    if (mo_cms[*pc] & (MO_CM_DEC)) {
        return mo_lex_accept_number_dec(x, k, r, pc);
    }

    //  如果遇到的是浮点数的点号前缀，那么当作浮点数来识别
    if ('.' == *pc) {
        return mo_lex_accept_number_float_postfix(x, k, r, pc);
    }

    //  无法识别的符号
    mo_result_errorf(r, 111, "");
    return pc;
}

static mo_byte              mo_lex_accept_escape_char(struct lex_t* x, struct result_t* r, mo_byte** pc)
{

}


MO_EXTERN struct token_t*  mo_lex_accept_string(struct lex_t* x, struct token_t* k, struct result_t* r)
{
    mo_byte* pc = x->pos;
    pc++;

    mo_byte first = *pc;

RETRY:
    while (0 == (mo_cms[(int)*pc] & MO_CM_STRING_FLAG)) {
        pc++;
    }

    if ('\"' == *pc) {
        pc++;
        k->id      = MO_TOKEN_STRING;
        k->text[0] = x->pos;
        k->text[1] = pc;
        x->pos = pc;
        return k;
    }

    if (*pc == '\\') {
        pc++;

        switch (*pc) {
            case '\\':
            case '\"':
            case 't':
            case 'v':
            case 'r':
            case 'n':
            case 'a':
            case 'b':
            case 'x':
            case 'u':
            case '\n':
                if (pc >= x->end) {
                    int ret = mo_lex_loadmore(x, r, pc);
                    if (MO_READ_OK == ret) {
                        goto RETRY; //  TODO bug
                    }

                    if (MO_READ_ERROR == ret) {
                        //  转义字符数据识别失败
                        mo_result_errorf(r, 111, "load cache failed");
                        return k;
                    }

                    if (MO_READ_EOF == ret) {
                        //  字符串没有正确地结束
                        mo_result_errorf(r, 111, "string do not end correctly");
                        return k;
                    }

                    //  不支持的返回码
                    mo_result_errorf(r, 111, "unsupported returned value");
                    return k;
                }


                //  如果遇到真换行:字符串不允许跨行
                mo_result_errorf(r, 111, "string should be closed brfore new line");
                return k;
            default:
                //  不支持的转义字符
                mo_result_errorf(r, 111, "unrecogenized espace char");
                return k;
        }
    }

    if (*pc == '\n') {
        //  遇到的并不是真的换行符号
        if (pc >= x->end) {
            int ret = mo_lex_loadmore(x, r, pc);
            if (MO_READ_OK == ret) {
                goto RETRY;
            }

            if (MO_READ_ERROR == ret) {
                mo_result_errorf(r, 111, "load cache failed");
                return k;
            }

            //  MO_READ_EOF 这种情况直接结束当前处理
            if (MO_READ_EOF == ret) {
                mo_result_errorf(r, 111, "string do not end correctly");
                return k;
            }

            //  不支持的返回码
            mo_result_errorf(r, 111, "unsupported returned value");
            return k;
        }

        //  如果遇到的就是真换行符号
        mo_result_errorf(r, 111, "string do not end correctly");
        return k;
    }

    //  程序不应该走到这里
    mo_result_errorf(r, 111, "inner-error:string");
    return k;
}

MO_EXTERN struct token_t*       mo_lex_accept_name(struct lex_t* x, struct token_t* k, struct result_t* r)
{
    mo_byte* pc = x->pos + 1;
    while (mo_result_ok(r)) {
        while (mo_cms[*pc] & (MO_CM_ALPHA | MO_CM_DEC | MO_CM_UDRLINE)) {
            pc++;
        }

        //  如果遇到换行，可能只是缓冲区不够了而已
        if ('\n' == *pc) {
            if (pc == x->end) {
                pc = mo_lex_loadmore(x, r, pc);
                if (pc != x->end) {
                    continue;   //  如果成功载入了更多的数据
                }
            }
        }

        return mo_token_as(k, MO_TOKEN_NAME, 0, x->pos, pc);
    }

    return mo_token_errorf(k, r, x->pos, 111, "");  //  TODO 是否可以使用错误栈
}




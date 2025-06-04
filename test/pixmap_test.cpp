// SPDX-License-Identifier: MIT
// Copyright (C) 2024 Artem Senichev <artemsen@gmail.com>

#include "pixmap_test.h"

TEST_F(PixmapTest, Create)
{
    struct pixmap pm;

    ASSERT_TRUE(pixmap_create(&pm, pixmap_argb, 123, 456));
    EXPECT_NE(pm.data, nullptr);
    EXPECT_EQ(pm.data[0], static_cast<size_t>(0));
    EXPECT_EQ(pm.format, pixmap_argb);
    EXPECT_EQ(pm.width, static_cast<size_t>(123));
    EXPECT_EQ(pm.height, static_cast<size_t>(456));

    pixmap_free(&pm);
}

TEST_F(PixmapTest, Fill)
{
    const argb_t clr = 0x12345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, clr,  clr,  0x13,
        0x20, clr,  clr,  0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };

    pixmap_fill(&pm, 1, 1, 2, 2, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, FillOutsideTL)
{
    const argb_t clr = 0x12345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        clr,  clr,  0x02, 0x03,
        clr,  clr,  0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_fill(&pm, -2, -2, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, FillOutsideBR)
{
    const argb_t clr = 0x12345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, clr,  clr,
        0x30, 0x31, clr,  clr,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_fill(&pm, 2, 2, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, InverseFill)
{
    const argb_t clr = 0x12345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        clr,  clr,  clr,  clr,
        clr,  0x11, 0x12, clr,
        clr,  0x21, 0x22, clr,
        clr,  clr,  clr,  clr,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_inverse_fill(&pm, 1, 1, 2, 2, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, InverseOutsideTL)
{
    const argb_t clr = 0x12345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0x00, 0x01, clr,  clr,
        0x10, 0x11, clr,  clr,
        clr,  clr,  clr,  clr,
        clr,  clr,  clr,  clr,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_inverse_fill(&pm, -2, -2, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, InverseOutsideBR)
{
    const argb_t clr = 0x12345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        clr,  clr,  clr,  clr,
        clr,  clr,  clr,  clr,
        clr,  clr,  0x22, 0x23,
        clr,  clr,  0x32, 0x33,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_inverse_fill(&pm, 2, 2, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, Grid)
{
    const argb_t clr1 = 0x12345678;
    const argb_t clr2 = 0x87654321;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        clr2, clr2, clr1, clr1,
        clr2, clr2, clr1, clr1,
        clr1, clr1, clr2, clr2,
        clr1, clr1, clr2, clr2,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_grid(&pm, -10, -10, 20, 20, 2, clr1, clr2);
    Compare(pm, expect);
}

TEST_F(PixmapTest, Mask)
{
    const argb_t clr = 0xffaaaaaa;

    // clang-format off
    argb_t src[] = {
        0xdddddddd, 0xcccccccc, 0xbbbbbbbb, 0xaaaaaaaa,
        0x11111111, 0xff000000, 0x80000000, 0x22222222,
        0x33333333, 0xaa111111, 0x00000000, 0x44444444,
        0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd,
    };
    const uint8_t mask[] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0xff, 0xff, 0x00,
        0x00, 0x80, 0x40, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };
    const argb_t expect[] = {
        0xdddddddd, 0xcccccccc, 0xbbbbbbbb, 0xaaaaaaaa,
        0x11111111, 0xffaaaaaa, 0xffaaaaaa, 0x22222222,
        0x33333333, 0xd46d6d6d, 0x40aaaaaa, 0x44444444,
        0xaaaaaaaa, 0xbbbbbbbb, 0xcccccccc, 0xdddddddd,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_apply_mask(&pm, 0, 0, mask, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, MaskOutsideTL)
{
    const argb_t clr = 0xffaaaaaa;

    // clang-format off
    argb_t src[] = {
        0xff000000, 0xff000000, 0xff000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0xff000000, 0xff000000, 0xff000000,
    };
    const uint8_t mask[] = {
        0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0x00, 0xff,
        0xff, 0x00, 0x00, 0xff,
        0xff, 0xff, 0xff, 0xff,
    };
    const argb_t expect[] = {
        0xff000000, 0xffaaaaaa, 0xff000000, 0xff000000,
        0xffaaaaaa, 0xffaaaaaa, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0xff000000, 0xff000000, 0xff000000,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_apply_mask(&pm, -2, -2, mask, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, MaskOutsideBR)
{
    const argb_t clr = 0xffaaaaaa;

    // clang-format off
    argb_t src[] = {
        0xff000000, 0xff000000, 0xff000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0xff000000, 0xff000000, 0xff000000,
    };
    const uint8_t mask[] = {
        0xff, 0xff, 0xff, 0xff,
        0xff, 0x00, 0x00, 0xff,
        0xff, 0x00, 0x00, 0xff,
        0xff, 0xff, 0xff, 0xff,
    };
    const argb_t expect[] = {
        0xff000000, 0xff000000, 0xff000000, 0xff000000,
        0xff000000, 0x00000000, 0x00000000, 0xff000000,
        0xff000000, 0x00000000, 0xffaaaaaa, 0xffaaaaaa,
        0xff000000, 0xff000000, 0xffaaaaaa, 0xff000000,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_apply_mask(&pm, 2, 2, mask, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, Copy)
{
    // clang-format off
    argb_t src[] = {
        0xaa, 0xbb,
        0xcc, 0xdd,
    };
    argb_t dst[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0xaa, 0xbb, 0x13,
        0x20, 0xcc, 0xdd, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    // clang-format on

    const struct pixmap pm_src = { pixmap_xrgb, 2, 2, src };
    struct pixmap pm_dst = { pixmap_argb, 4, 4, dst };
    pixmap_copy(&pm_src, &pm_dst, 1, 1);
    Compare(pm_dst, expect);
}

TEST_F(PixmapTest, CopyOutsideTL)
{
    // clang-format off
    argb_t src[] = {
        0xaa, 0xbb,
        0xcc, 0xdd,
    };
    argb_t dst[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0xdd, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    // clang-format on

    const struct pixmap pm_src = { pixmap_xrgb, 2, 2, src };
    struct pixmap pm_dst = { pixmap_argb, 4, 4, dst };
    pixmap_copy(&pm_src, &pm_dst, -1, -1);
    Compare(pm_dst, expect);
}

TEST_F(PixmapTest, CopyOutsideBR)
{
    // clang-format off
    argb_t src[] = {
        0xaa, 0xbb,
        0xcc, 0xdd,
    };
    argb_t dst[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0xaa,
    };
    // clang-format on

    const struct pixmap pm_src = { pixmap_xrgb, 2, 2, src };
    struct pixmap pm_dst = { pixmap_argb, 4, 4, dst };
    pixmap_copy(&pm_src, &pm_dst, 3, 3);
    Compare(pm_dst, expect);
}

TEST_F(PixmapTest, CopyAlpha)
{
    // clang-format off
    argb_t src[] = {
        0xffaaaaaa, 0x80aaaaaa,
        0x40aaaaaa, 0x00aaaaaa,
    };
    argb_t dst[] = {
        0x00000000, 0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0x55555555, 0x66666666, 0x77777777,
        0x88888888, 0x99999999, 0xaaaaaaaa, 0xbbbbbbbb,
        0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffffffff,
    };
    const argb_t expect[] = {
        0x00000000, 0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0xffaaaaaa, 0xb2969696, 0x77777777,
        0x88888888, 0xb29f9f9f, 0xaaaaaaaa, 0xbbbbbbbb,
        0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffffffff,
    };
    // clang-format on

    const struct pixmap pm_src = { pixmap_argb, 2, 2, src };
    struct pixmap pm_dst = { pixmap_argb, 4, 4, dst };
    pixmap_copy(&pm_src, &pm_dst, 1, 1);
    Compare(pm_dst, expect);
}

TEST_F(PixmapTest, CopyAlphaOutsideTL)
{
    // clang-format off
    argb_t src[] = {
        0x00aaaaaa, 0x40bbbbbb,
        0x80cccccc, 0xffdddddd,
    };
    argb_t dst[] = {
        0x00000000, 0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0x55555555, 0x66666666, 0x77777777,
        0x88888888, 0x99999999, 0xaaaaaaaa, 0xbbbbbbbb,
        0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffffffff,
    };
    const argb_t expect[] = {
        0xffdddddd, 0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0x55555555, 0x66666666, 0x77777777,
        0x88888888, 0x99999999, 0xaaaaaaaa, 0xbbbbbbbb,
        0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffffffff,
    };
    // clang-format on

    const struct pixmap pm_src = { pixmap_argb, 2, 2, src };
    struct pixmap pm_dst = { pixmap_argb, 4, 4, dst };
    pixmap_copy(&pm_src, &pm_dst, -1, -1);
    Compare(pm_dst, expect);
}

TEST_F(PixmapTest, CopyAlphaOutsideBL)
{
    // clang-format off
    argb_t src[] = {
        0xffaaaaaa, 0x80aaaaaa,
        0x40aaaaaa, 0x00aaaaaa,
    };
    argb_t dst[] = {
        0x00000000, 0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0x55555555, 0x66666666, 0x77777777,
        0x88888888, 0x99999999, 0xaaaaaaaa, 0xbbbbbbbb,
        0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffffffff,
    };
    const argb_t expect[] = {
        0x00000000, 0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0x55555555, 0x66666666, 0x77777777,
        0x88888888, 0x99999999, 0xaaaaaaaa, 0xbbbbbbbb,
        0xcccccccc, 0xdddddddd, 0xeeeeeeee, 0xffaaaaaa,
    };
    // clang-format on

    const struct pixmap pm_src = { pixmap_argb, 2, 2, src };
    struct pixmap pm_dst = { pixmap_argb, 4, 4, dst };
    pixmap_copy(&pm_src, &pm_dst, 3, 3);
    Compare(pm_dst, expect);
}

TEST_F(PixmapTest, Rect)
{
    const argb_t clr = 0xff345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        clr,  clr,  clr,  clr,
        clr,  0x11, 0x12, clr,
        clr,  0x21, 0x22, clr,
        clr,  clr,  clr,  clr,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };

    pixmap_rect(&pm, 0, 0, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, RectOutsideTL)
{
    const argb_t clr = 0xff345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0x00, clr,  0x02, 0x03,
        clr,  clr,  0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_rect(&pm, -2, -2, 4, 4, clr);
    Compare(pm, expect);
}

TEST_F(PixmapTest, RectOutsideBR)
{
    const argb_t clr = 0xff345678;

    // clang-format off
    argb_t src[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, 0x22, 0x23,
        0x30, 0x31, 0x32, 0x33,
    };
    const argb_t expect[] = {
        0x00, 0x01, 0x02, 0x03,
        0x10, 0x11, 0x12, 0x13,
        0x20, 0x21, clr,  clr,
        0x30, 0x31, clr,  0x33,
    };
    // clang-format on

    struct pixmap pm = { pixmap_argb, 4, 4, src };
    pixmap_rect(&pm, 2, 2, 4, 4, clr);
    Compare(pm, expect);
}

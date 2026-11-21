// The 20 built-in templates.
// 1-9 are based on the styles Myriam sent, 10-20 were designed for the mod.
#include "Deco.hpp"

namespace deco {
namespace {

// ---------- small builders ----------
struct PB {
    Piece p;
    PB(int id) { p.id = id; p.z = ZT2; p.zo = 6; }
    PB& at(float dx, float dy) { p.dx = dx; p.dy = dy; return *this; }
    PB& sc(float s) { p.sx = p.sy = s; return *this; }
    PB& sc(float x, float y) { p.sx = x; p.sy = y; return *this; }
    PB& r(float d) { p.rot = d; return *this; }
    PB& c(int s) { p.slot = s; return *this; }
    PB& c2(int s) { p.slot2 = s; return *this; }
    PB& z(int z, int zo) { p.z = z; p.zo = zo; return *this; }
    PB& zo(int zo) { p.zo = zo; return *this; }
    PB& jit(float pos, float rot = 0, float scale = 0) { p.jPos = pos; p.jRot = rot; p.jScale = scale; return *this; }
    PB& flip() { p.randFlip = true; return *this; }
    PB& fx() { p.fx = true; return *this; }
    PB& fy() { p.fy = true; return *this; }
    PB& anim() { p.anim = true; return *this; }
    PB& v(float v) { p.v = v; return *this; }
    operator Piece() const { return p; }
};
// a 15x15 white square sized in units (w x h)
PB sq(float w, float h) { PB b(916); b.sc(w / 15.f, h / 15.f); return b; }

Cluster C(std::initializer_list<Piece> ps, float w = 1.f) { Cluster c; c.pieces = ps; c.weight = w; return c; }

Feature fill(int slot, int z = ZT2, int zo = 0, float v = 1.f) {
    Feature f; f.type = FType::Fill; f.slot = slot; f.z = z; f.zo = zo; f.v = v; return f;
}
Feature outline(int slot, float thick, float inset = 0, int zo = 5, unsigned sides = 0xF) {
    Feature f; f.type = FType::Outline; f.slot = slot; f.thick = thick; f.inset = inset; f.z = ZT2; f.zo = zo; f.sides = sides; return f;
}
Feature glow(int slot, float size, bool inward = false, int z = ZB1, int zo = 0, unsigned sides = 0xF) {
    Feature f; f.type = FType::Glow; f.slot = slot; f.glowSize = size; f.inward = inward; f.z = z; f.zo = zo; f.sides = sides;
    if (inward) { f.z = ZT2; f.zo = zo ? zo : 1; }
    return f;
}
Feature extrude(float ex, float ey, int top, int side, int line = -1, float lt = 1.5f, int zo = 4) {
    Feature f; f.type = FType::Extrude; f.ex = ex; f.ey = ey; f.slotTop = top; f.slotSide = side; f.lineSlot = line; f.lineThick = lt;
    f.z = ZT2; f.zo = zo; return f;
}
Feature surf(Side s, float prob, std::initializer_list<Cluster> cs, int spacing = 1, bool orient = true) {
    Feature f; f.type = FType::Surface; f.side = s; f.prob = prob; f.opts = cs; f.spacing = spacing; f.orient = orient;
    f.priority = 1; return f;
}
Feature corner(float prob, unsigned mask, std::initializer_list<Cluster> cs, bool orient = false, bool concave = false) {
    Feature f; f.type = FType::Corner; f.prob = prob; f.cornerMask = mask; f.opts = cs; f.orient = orient;
    f.concave = concave; f.convex = !concave; f.priority = 1; return f;
}
Feature inter(Pattern pat, float prob, std::initializer_list<Cluster> cs, int px = 2, int py = 2, int minDepth = 0) {
    Feature f; f.type = FType::Interior; f.pat = pat; f.prob = prob; f.opts = cs; f.px = px; f.py = py; f.minDepth = minDepth;
    f.orient = false; f.priority = 2; return f;
}
Feature band(float dist, float thick, int slot, float inset = 0, int zo = 4) {
    Feature f; f.type = FType::Band; f.dist = dist; f.thick = thick; f.slot = slot; f.inset = inset; f.z = ZT2; f.zo = zo; return f;
}
Feature scatter(float perCell, int margin, std::initializer_list<Cluster> cs) {
    Feature f; f.type = FType::Scatter; f.perCell = perCell; f.margin = margin; f.opts = cs; f.priority = 3; return f;
}
Feature withChance(Feature f, float c) { f.chance = c; return f; }
Feature withJ(Feature f, float j) { f.probJ = j; return f; }

Slot S(int r, int g, int b, bool blend = false, char const* name = "") { Slot s; s.c = { r, g, b }; s.blend = blend; s.name = name; return s; }

constexpr unsigned TL = 1, TR = 2, BL = 4, BR = 8, TOPS = TL | TR, BOTS = BL | BR;
constexpr unsigned SIDES_LRB = (1u << 1) | (1u << 2) | (1u << 3);

std::vector<Template> make() {
    std::vector<Template> v;

    // 1 ---------------------------------------------------------------- Neon Grid (Myriam #1)
    {
        Template t; t.id = "neon-grid"; t.name = "Neon Grid"; t.by = "Myriam";
        t.desc = "Pink & cyan neon outlines, 3D tops, soft inner glow";
        t.palette = { S(255, 45, 170, false, "Neon A"), S(40, 225, 255, false, "Neon B"), S(22, 8, 30, false, "Fill"),
                      S(255, 30, 160, true, "Glow A"), S(30, 200, 255, true, "Glow B"), S(110, 20, 85, false, "Top") };
        t.swaps = { { 0, 1 }, { 3, 4 } }; t.slotSwap = 0.45f; t.hueVar = 6;
        t.features = {
            fill(2),
            glow(3, 1.1f, true),
            extrude(7, 7, 5, 2, 0, 2.f),
            outline(0, 2.5f),
            glow(3, 1.0f),
            withJ(inter(Pattern::Random, 0.22f, {
                C({ PB(917).c(3).sc(0.9f).jit(7).zo(2) }),
                C({ sq(10, 1.2f).c(0).at(0, -4).jit(5).zo(2).v(0.7f) }, 0.6f),
            }), 0.1f),
            withChance(surf(Side::Top, 0.18f, { C({ PB(1766).c(1).sc(1.4f).at(0, 5.6f) }) }, 2), 0.7f),
            withChance(scatter(0.05f, 2, { C({ PB(917).c(1).sc(0.8f).jit(6) }), C({ PB(917).c(0).sc(0.6f).jit(6) }) }), 0.6f),
        };
        v.push_back(t);
    }
    // 2 ---------------------------------------------------------------- Pixel Pop City (Myriam #2)
    {
        Template t; t.id = "pixel-pop"; t.name = "Pixel Pop City"; t.by = "Myriam";
        t.desc = "Bright pink/blue pixel buildings with bold borders";
        t.palette = { S(235, 50, 195, false, "Main"), S(130, 0, 110, false, "Border"), S(255, 120, 235, false, "Light"),
                      S(80, 150, 230, false, "Main B"), S(30, 105, 205, false, "Border B"), S(150, 195, 255, false, "Light B") };
        t.swaps = { { 0, 3 }, { 1, 4 }, { 2, 5 } }; t.slotSwap = 0.4f; t.hueVar = 5;
        t.features = {
            fill(0),
            extrude(6, 8, 2, 1, 1, 2.5f),
            outline(1, 4.f),
            band(4, 5, 2, 4),
            withJ(inter(Pattern::Random, 0.35f, {
                C({ PB(917).c(1).sc(1.1f).jit(8).zo(3) }),
                C({ PB(917).c(1).sc(0.7f).jit(8).zo(3), PB(917).c(1).sc(0.5f).at(6, -5).jit(6).zo(3) }),
                C({ sq(7, 7).c(2).jit(7).zo(3) }, 0.4f),
            }, 2, 2, 1), 0.1f),
            surf(Side::Left, 0.22f, { C({ sq(14, 14).c(1).at(0, 5).zo(2), sq(9, 9).c(2).at(0, 5).zo(3) }) }, 2),
            surf(Side::Right, 0.22f, { C({ sq(14, 14).c(1).at(0, 5).zo(2), sq(9, 9).c(2).at(0, 5).zo(3) }) }, 2),
            withChance(scatter(0.05f, 2, { C({ PB(917).c(1).sc(1.f).jit(5) }), C({ PB(917).c(4).sc(0.7f).jit(5) }) }), 0.7f),
        };
        v.push_back(t);
    }
    // 3 ---------------------------------------------------------------- Blue Spire Glow (Myriam #3)
    {
        Template t; t.id = "blue-spire"; t.name = "Blue Spire Glow"; t.by = "Myriam";
        t.desc = "Dark blue blocks, glowing edges, crystal tips on corners";
        t.palette = { S(130, 175, 255, false, "Edge"), S(40, 95, 255, true, "Glow"), S(4, 8, 35, false, "Fill"), S(215, 235, 255, false, "Tips") };
        t.hueVar = 8;
        t.features = {
            fill(2),
            glow(1, 0.9f, true),
            outline(0, 2.f),
            glow(1, 1.5f),
            corner(0.85f, TOPS, { C({ PB(1766).c(3).sc(2.2f).at(-7, 8.8f) }), C({ PB(1766).c(3).sc(1.6f).at(-6, 6.4f), PB(1766).c(0).sc(1.1f).at(-13, 4.4f) }, 0.5f) }),
            withChance(surf(Side::Top, 0.14f, { C({ PB(1766).c(3).sc(1.2f).at(0, 4.8f).jit(4) }) }), 0.8f),
            inter(Pattern::Random, 0.1f, { C({ PB(1764).c(0).sc(0.45f).jit(9).zo(2) }) }),
            withChance(scatter(0.04f, 2, { C({ PB(1764).c(3).sc(0.4f).jit(8) }) }), 0.6f),
        };
        v.push_back(t);
    }
    // 4 ---------------------------------------------------------------- Golden Fortress (Myriam #4)
    {
        Template t; t.id = "golden-fortress"; t.name = "Golden Fortress"; t.by = "Myriam";
        t.desc = "Dark towers with gold trim, lit windows and 3D roofs";
        t.palette = { S(230, 190, 110, false, "Gold"), S(28, 20, 14, false, "Fill"), S(255, 228, 140, false, "Window"),
                      S(255, 165, 55, true, "Warm glow"), S(62, 46, 30, false, "Roof"), S(95, 62, 30, false, "Frame") };
        t.hueVar = 5;
        Cluster window = C({ sq(19, 19).c(0).zo(6), sq(16, 16).c(2).zo(7), sq(16, 1.6f).c(5).zo(8), sq(1.6f, 16).c(5).zo(8),
                             PB(503).c(3).sc(0.6f, 0.6f).at(0, -5).z(ZT2, 9) });
        Cluster vent = C({ sq(1.8f, 18).c(0).at(-6, 0).zo(6), sq(1.8f, 18).c(0).at(0, 0).zo(6), sq(1.8f, 18).c(0).at(6, 0).zo(6) }, 0.35f);
        Cluster darkWin = C({ sq(19, 19).c(0).zo(6), sq(16, 16).c(5).zo(7), sq(16, 1.6f).c(1).zo(8), sq(1.6f, 16).c(1).zo(8) }, 0.4f);
        Cluster spike = C({ PB(149).c(0).sc(0.78f).at(0, 11.7f).zo(3), PB(149).c(1).sc(0.62f).at(0, 10.4f).zo(4) });
        t.features = {
            fill(1),
            extrude(9, 9, 4, 1, 0, 1.6f),
            outline(0, 2.f),
            withChance(glow(3, 0.9f), 0.6f),
            withJ(inter(Pattern::Windows, 0.7f, { window, vent, darkWin }, 2, 2), 0.15f),
            withChance(surf(Side::Top, 0.22f, { spike }, 2), 0.85f),
            withChance(surf(Side::Bottom, 0.25f, { spike }, 2), 0.85f),
            withChance(surf(Side::Left, 0.15f, { C({ sq(22, 11).c(0).at(0, 7).zo(2), sq(19, 8).c(4).at(0, 7).zo(3) }) }, 2), 0.6f),
            withChance(surf(Side::Right, 0.15f, { C({ sq(22, 11).c(0).at(0, 7).zo(2), sq(19, 8).c(4).at(0, 7).zo(3) }) }, 2), 0.6f),
        };
        v.push_back(t);
    }
    // 5 ---------------------------------------------------------------- Techno Panels (Myriam #5)
    {
        Template t; t.id = "techno-panels"; t.name = "Techno Panels"; t.by = "Myriam";
        t.desc = "Red & blue machine panels with stripes, pads and cogs";
        t.palette = { S(255, 70, 95, false, "Neon A"), S(90, 185, 255, false, "Neon B"), S(34, 24, 44, false, "Fill"),
                      S(255, 125, 145, false, "Stripe"), S(255, 60, 90, true, "Glow"), S(95, 235, 255, false, "Pad") };
        t.swaps = { { 0, 1 } }; t.slotSwap = 0.4f;
        Cluster stripes = C({ sq(18, 3.5f).c(3).r(-45).at(-7, 0).zo(3), sq(18, 3.5f).c(3).r(-45).at(0, 0).zo(3), sq(18, 3.5f).c(3).r(-45).at(7, 0).zo(3),
                              sq(26, 26).c(2).v(1.4f).zo(2) });
        Cluster pads = C({ PB(1767).c(5).sc(1.6f, 1.6f).at(-6, 0).zo(3), PB(1767).c(5).sc(1.6f, 1.6f).at(6, 0).zo(3) }, 0.6f);
        t.features = {
            fill(2),
            outline(0, 2.5f),
            outline(0, 1.f, 6.f, 4),
            withJ(inter(Pattern::Windows, 0.7f, { stripes, pads }, 3, 2, 1), 0.1f),
            surf(Side::Top, 0.3f, { C({ PB(50).c(5).sc(0.5f).at(0, 0).z(ZT1, 0) }) }, 2),
            surf(Side::Bottom, 0.2f, { C({ PB(50).c(5).sc(0.5f).at(0, 0).z(ZT1, 0) }) }, 2),
            withChance(corner(0.3f, 0xF, { C({ PB(97).c(1).sc(0.6f).at(-2, -2).anim().zo(7) }) }), 0.7f),
            glow(4, 0.8f),
        };
        v.push_back(t);
    }
    // 6 ---------------------------------------------------------------- Rooftop Day (Myriam #6)
    {
        Template t; t.id = "rooftop-day"; t.name = "Rooftop Day"; t.by = "Myriam";
        t.desc = "Sunny city walls, windows, cornice and railings";
        t.palette = { S(255, 242, 232, false, "Wall"), S(125, 82, 62, false, "Cornice"), S(140, 195, 245, false, "Glass"),
                      S(72, 62, 72, false, "Frame"), S(40, 42, 62, false, "Railing"), S(215, 238, 255, false, "Shine") };
        t.hueVar = 4;
        Cluster win = C({ sq(18, 22).c(3).zo(5), sq(15, 19).c(2).zo(6), sq(3, 16).c(5).r(15).at(-3, 0).zo(7), sq(22, 3).c(1).at(0, -12).zo(7) });
        Cluster win2 = C({ sq(18, 22).c(3).zo(5), sq(15, 19).c(2).zo(6), sq(15, 1.5f).c(3).zo(7), sq(22, 3).c(1).at(0, -12).zo(7) }, 0.6f);
        t.features = {
            fill(0),
            outline(1, 1.5f, 0, 5, SIDES_LRB),
            band(0, 7, 1),
            band(7, 2, 3, 0, 5),
            withJ(inter(Pattern::Windows, 0.8f, { win, win2 }, 2, 2, 1), 0.1f),
            withChance(surf(Side::Top, 1.f, { C({ sq(2, 14).c(4).at(-13, 7).zo(3), sq(30, 1.8f).c(4).at(0, 14).zo(3), sq(30, 1.4f).c(4).at(0, 7.5f).zo(3) }) }), 0.75f),
            withChance(surf(Side::Top, 0.1f, { C({ sq(1.5f, 18).c(4).at(0, 9).zo(2), PB(1764).c(4).sc(0.5f).at(0, 18).zo(2) }) }, 3), 0.4f),
        };
        v.push_back(t);
    }
    // 7 ---------------------------------------------------------------- Flat Modern (Myriam #7)
    {
        Template t; t.id = "flat-modern"; t.name = "Flat Modern"; t.by = "Myriam";
        t.desc = "Clean navy blocks, white diamonds, lavender light pillars";
        t.palette = { S(42, 52, 82, false, "Navy"), S(240, 240, 255, false, "White"), S(195, 165, 255, false, "Lavender"),
                      S(175, 135, 255, true, "Light"), S(72, 82, 112, false, "Border") };
        t.features = {
            fill(0),
            outline(4, 1.5f),
            inter(Pattern::Random, 0.25f, { C({ PB(53).c(1).sc(0.3f).zo(3) }) }),
            withChance(surf(Side::Top, 0.35f, { C({ sq(22, 3.5f).c(0).at(0, 4), sq(22, 3.5f).c(0).at(0, 9.5f), sq(22, 3.5f).c(0).at(0, 15) }) }, 2), 0.8f),
            surf(Side::Right, 0.3f, { C({ PB(1293).c(3).sc(1.f, 2.f).at(0, 7.5f).z(ZB1, 0) }) }, 2, false),
            surf(Side::Left, 0.3f, { C({ PB(1293).c(3).sc(1.f, 2.f).at(0, 7.5f).z(ZB1, 0) }) }, 2, false),
            withChance(surf(Side::Bottom, 0.3f, { C({ PB(1766).c(0).sc(1.6f).at(0, 6.4f) }) }), 0.7f),
            corner(0.3f, TOPS, { C({ PB(53).c(1).sc(0.35f).at(-8, -8).zo(4) }) }),
        };
        v.push_back(t);
    }
    // 8 ---------------------------------------------------------------- Mystic Outline (Myriam #8)
    {
        Template t; t.id = "mystic-outline"; t.name = "Mystic Outline"; t.by = "Myriam";
        t.desc = "Thin cyan lines, purple runes, tiny squares and canes";
        t.palette = { S(130, 240, 255, false, "Cyan"), S(170, 90, 255, false, "Purple"), S(8, 10, 28, false, "Fill"),
                      S(90, 220, 255, true, "Cyan glow"), S(150, 70, 255, true, "Purple glow") };
        t.swaps = { { 0, 1 }, { 3, 4 } }; t.slotSwap = 0.25f;
        t.features = {
            fill(2),
            outline(0, 1.5f),
            glow(3, 0.7f),
            withJ(inter(Pattern::Random, 0.3f, {
                C({ sq(14, 2).c(1).jit(7).zo(3) }),
                C({ sq(2, 16).c(1).jit(7).zo(3) }),
                C({ PB(917).c(0).sc(0.85f).zo(3), PB(917).c(2).sc(0.6f).zo(4) }, 0.7f),
            }), 0.1f),
            corner(0.5f, TOPS, { C({ PB(1766).c(0).sc(2.6f).at(-11, 10.4f), PB(1766).c(1).sc(1.6f).at(-11, 9.6f).zo(7) }) }),
            withChance(surf(Side::Top, 0.1f, { C({ PB(1766).c(0).sc(1.5f).at(-4, 6), PB(1766).c(0).sc(1.5f).at(5, 6) }) }), 0.7f),
            surf(Side::Left, 0.12f, { C({ sq(16, 11).c(1).at(0, 3).v(0.85f).zo(2), sq(12, 2).c(0).r(-35).at(0, 3).zo(3) }) }, 3),
            surf(Side::Right, 0.12f, { C({ sq(16, 11).c(1).at(0, 3).v(0.85f).zo(2), sq(12, 2).c(0).r(35).at(0, 3).zo(3) }) }, 3),
            withChance(scatter(0.06f, 3, {
                C({ sq(1.5f, 40).c(0).at(0, -5), PB(1831).c(0).sc(0.4f).at(6, 18) }),
                C({ PB(917).c(0).sc(0.7f), PB(917).c(2).sc(0.45f).zo(7) }, 1.5f),
            }), 0.8f),
        };
        v.push_back(t);
    }
    // 9 ---------------------------------------------------------------- Circuit Glow (Myriam #9)
    {
        Template t; t.id = "circuit-glow"; t.name = "Circuit Glow"; t.by = "Myriam";
        t.desc = "Bright white blocks, purple aura, circuit traces and vents";
        t.palette = { S(246, 244, 255, false, "White"), S(150, 60, 255, true, "Aura"), S(45, 32, 75, false, "Trace"),
                      S(232, 222, 255, false, "Edge"), S(222, 216, 240, false, "Top"), S(198, 190, 222, false, "Side") };
        t.features = {
            fill(0),
            extrude(10, 10, 4, 5, 3, 1.5f),
            outline(3, 1.5f, -1.5f),
            glow(1, 2.f),
            withJ(inter(Pattern::Random, 0.32f, {
                C({ sq(16, 1.6f).c(2).at(-3, -4).zo(3), sq(1.6f, 12).c(2).at(5, 2).zo(3), PB(1764).c(2).sc(0.6f).at(-11, -4).zo(4), PB(1764).c(2).sc(0.6f).at(5, 8).zo(4) }),
                C({ sq(1.6f, 16).c(2).at(-5, 0).zo(3), sq(1.6f, 16).c(2).at(0, 0).zo(3), sq(1.6f, 16).c(2).at(5, 0).zo(3) }, 0.7f),
                C({ PB(1764).c(2).sc(0.35f).at(-5, 0).zo(3), PB(1764).c(2).sc(0.35f).at(0, 0).zo(3), PB(1764).c(2).sc(0.35f).at(5, 0).zo(3) }, 0.6f),
                C({ sq(14, 14).c(2).zo(3), sq(11, 11).c(0).zo(4), PB(1764).c(2).sc(0.6f).zo(5) }, 0.5f),
            }), 0.1f),
            corner(0.35f, 0xF, { C({ PB(1830).c(2).sc(0.6f).at(-9, -9).zo(3) }) }, true, true),
        };
        v.push_back(t);
    }
    // 10 --------------------------------------------------------------- Amethyst Crystal
    {
        Template t; t.id = "amethyst"; t.name = "Amethyst Crystal"; t.by = "Claude";
        t.desc = "Deep purple rock with glowing crystal clusters";
        t.palette = { S(40, 10, 70, false, "Rock"), S(190, 95, 255, false, "Crystal"), S(238, 195, 255, false, "Shine"),
                      S(160, 60, 255, true, "Glow"), S(95, 42, 145, false, "Edge") };
        t.hueVar = 10;
        Cluster c1 = C({ PB(151).c(1).sc(0.75f).at(-3, 15).r(-8).jit(2, 6, 0.2f), PB(153).c(2).sc(0.7f).at(5, 6.6f).r(14).jit(2, 6, 0.2f) });
        Cluster c2 = C({ PB(152).c(1).sc(0.7f).at(0, 10.5f).jit(3, 10, 0.2f), PB(153).c(2).sc(0.55f).at(-6, 5).r(-20).jit(2, 6) });
        Cluster c3 = C({ PB(153).c(2).sc(0.8f).at(0, 7.6f).jit(5, 15, 0.2f) }, 0.7f);
        t.features = {
            fill(0),
            glow(3, 0.8f, true),
            outline(4, 2.f),
            glow(3, 1.2f),
            withJ(surf(Side::Top, 0.55f, { c1, c2, c3 }), 0.15f),
            withChance(surf(Side::Bottom, 0.35f, { C({ PB(152).c(1).sc(0.6f).at(0, 9).jit(3, 8, 0.2f) }), c3 }), 0.8f),
            inter(Pattern::Random, 0.2f, { C({ PB(53).c(2).sc(0.3f).jit(8).zo(3) }), C({ PB(1766).c(1).sc(1.f).jit(8, 30).zo(3) }) }),
            withChance(scatter(0.05f, 2, { C({ PB(53).c(2).sc(0.25f).jit(6) }), C({ PB(54).c(2).sc(0.22f).jit(6, 30) }) }), 0.7f),
        };
        v.push_back(t);
    }
    // 11 --------------------------------------------------------------- Miku Neon
    {
        Template t; t.id = "miku-neon"; t.name = "Miku Neon"; t.by = "Claude";
        t.desc = "Teal & pink stage lights, equalizer bars, music notes";
        t.palette = { S(57, 197, 187, false, "Teal"), S(255, 100, 180, false, "Pink"), S(14, 24, 34, false, "Fill"),
                      S(40, 220, 200, true, "Teal glow"), S(235, 255, 255, false, "White"), S(255, 80, 170, true, "Pink glow") };
        t.swaps = { { 0, 1 }, { 3, 5 } }; t.slotSwap = 0.3f;
        auto bars = [](float a, float b, float c) {
            return C({ sq(4, a).c(0).at(-8, -14 + a / 2).zo(3), sq(4, b).c(1).at(0, -14 + b / 2).zo(3), sq(4, c).c(0).at(8, -14 + c / 2).zo(3) });
        };
        t.features = {
            fill(2),
            outline(0, 2.5f),
            outline(1, 1.f, 5.f, 4),
            glow(3, 1.f),
            withJ(inter(Pattern::Rows, 0.5f, { bars(10, 18, 7), bars(16, 9, 20), bars(6, 14, 11), bars(20, 12, 16) }, 2, 2, 1), 0.15f),
            withChance(surf(Side::Top, 0.22f, { C({ PB(60).c(4).sc(0.5f).at(0, 9).jit(3, 12) }), C({ PB(1767).c(1).sc(1.5f).at(0, 4).jit(4) }) }), 0.85f),
            withChance(scatter(0.06f, 2, { C({ PB(60).c(1).sc(0.4f).jit(6, 15) }), C({ PB(60).c(0).sc(0.35f).jit(6, 15) }) }), 0.8f),
        };
        v.push_back(t);
    }
    // 12 --------------------------------------------------------------- Fortress Stone
    {
        Template t; t.id = "fortress-stone"; t.name = "Fortress Stone"; t.by = "Claude";
        t.desc = "Grey brick walls, battlements, chains and torches";
        t.palette = { S(92, 92, 104, false, "Stone"), S(40, 40, 48, false, "Mortar"), S(152, 152, 168, false, "Cap"),
                      S(56, 56, 64, false, "Crack"), S(255, 150, 50, true, "Torch glow"), S(255, 205, 95, false, "Flame") };
        t.hueVar = 4;
        t.features = {
            fill(0),
            withJ(inter(Pattern::Every, 0.85f, { C({ sq(30, 1.6f).c(1).at(0, 0).zo(2), sq(30, 1.6f).c(1).at(0, 15).zo(2),
                                                    sq(1.6f, 15).c(1).at(-7, 7.5f).zo(2), sq(1.6f, 15).c(1).at(8, -7.5f).zo(2) }) }), 0.1f),
            outline(1, 2.5f),
            band(0, 5, 2, 2.5f),
            inter(Pattern::Random, 0.12f, { C({ PB(1369).c(3).sc(0.5f).jit(6, 180).zo(3) }), C({ PB(1371).c(3).sc(0.5f).jit(6, 180).zo(3) }) }),
            withChance(surf(Side::Top, 1.f, { C({ sq(15, 12).c(0).at(0, 6).zo(2), sq(15, 2.5f).c(2).at(0, 11).zo(3), sq(1.5f, 12).c(1).at(-7, 6).zo(3), sq(1.5f, 12).c(1).at(7, 6).zo(3) }) }, 2), 0.75f),
            withChance(surf(Side::Bottom, 0.2f, { C({ PB(41).c(1).sc(0.45f).at(0, 15.6f) }), C({ PB(106).c(1).sc(0.45f).at(0, 15) }) }, 2, false), 0.8f),
            surf(Side::Left, 0.1f, { C({ sq(3, 12).c(1).at(0, 1).zo(3), PB(1766).c(5).sc(1.3f, 1.8f).at(0, 10).zo(4), PB(504).c(4).sc(1.2f).at(0, 10).z(ZB1, 0) }) }, 3, false),
            surf(Side::Right, 0.1f, { C({ sq(3, 12).c(1).at(0, 1).zo(3), PB(1766).c(5).sc(1.3f, 1.8f).at(0, 10).zo(4), PB(504).c(4).sc(1.2f).at(0, 10).fx().z(ZB1, 0) }) }, 3, false),
        };
        v.push_back(t);
    }
    // 13 --------------------------------------------------------------- Overgrown Forest
    {
        Template t; t.id = "overgrown"; t.name = "Overgrown Forest"; t.by = "Claude";
        t.desc = "Earth blocks, grassy tops, bushes, flowers and vines";
        t.palette = { S(92, 62, 42, false, "Earth"), S(95, 205, 75, false, "Grass"), S(42, 135, 52, false, "Dark grass"),
                      S(255, 125, 185, false, "Flower"), S(62, 155, 62, false, "Moss"), S(32, 92, 42, false, "Vine") };
        t.hueVar = 8;
        t.features = {
            fill(0),
            outline(5, 2.f, 0, 5, SIDES_LRB),
            band(0, 7, 4),
            surf(Side::Top, 0.85f, {
                C({ PB(406).c(1).sc(1.1f).at(0, 7.5f).flip().jit(5, 0, 0.2f) }),
                C({ PB(407).c(1).sc(0.8f).at(4, 4.5f).flip().jit(5), PB(408).c(2).sc(0.8f).at(-6, 3).flip() }),
                C({ PB(908).c(2).sc(0.45f).at(0, 4.5f).flip().jit(4, 0, 0.15f) }, 0.6f),
                C({ PB(909).c(1).sc(0.5f).at(0, 4).flip().jit(4) }, 0.5f),
                C({ PB(939).c(3).c2(1).sc(0.6f).at(0, 1).jit(5) }, 0.25f),
            }),
            withChance(surf(Side::Bottom, 0.3f, { C({ PB(1756).c(5).sc(0.6f, 1.2f).at(0, 18).jit(3, 6), PB(1764).c(2).sc(0.6f).at(0, 35) }),
                                                  C({ PB(1756).c(5).sc(0.6f, 0.7f).at(0, 10.5f).jit(3, 6) }) }, 1, false), 0.8f),
            inter(Pattern::Random, 0.15f, { C({ PB(50).c(5).sc(0.18f).jit(8).zo(3) }), C({ PB(405).c(4).sc(0.2f).jit(8).zo(3) }) }, 2, 2, 1),
            surf(Side::Left, 0.15f, { C({ PB(1756).c(5).sc(0.6f, 1.f).at(0, 2).jit(2, 5) }) }, 1, false),
            surf(Side::Right, 0.15f, { C({ PB(1756).c(5).sc(0.6f, 1.f).at(0, 2).jit(2, 5) }) }, 1, false),
        };
        v.push_back(t);
    }
    // 14 --------------------------------------------------------------- Frozen Peak
    {
        Template t; t.id = "frozen-peak"; t.name = "Frozen Peak"; t.by = "Claude";
        t.desc = "Icy blocks with snow caps, icicles and snowflakes";
        t.palette = { S(150, 212, 255, false, "Ice"), S(246, 251, 255, false, "Snow"), S(40, 92, 165, false, "Deep"),
                      S(120, 205, 255, true, "Glow"), S(205, 238, 255, false, "Icicle") };
        t.features = {
            fill(0),
            glow(2, 1.3f, true, ZT2, 1, 0xE),
            outline(1, 1.5f),
            surf(Side::Top, 1.f, { C({ PB(1893).c(1).at(0, -6).zo(7) }), C({ PB(1896).c(1).at(0, -6).zo(7) }), C({ PB(1897).c(1).at(0, -6).zo(7) }), C({ PB(1898).c(1).at(0, -6).zo(7) }) }),
            surf(Side::Bottom, 0.5f, { C({ PB(149).c(4).sc(0.32f, 0.7f).at(0, 10.5f).jit(3, 0, 0.3f) }), C({ PB(149).c(4).sc(0.25f, 0.5f).at(-6, 7.5f), PB(149).c(4).sc(0.2f, 0.4f).at(6, 6) }) }),
            withChance(glow(3, 0.8f), 0.7f),
            withChance(scatter(0.05f, 2, { C({ PB(1765).c(1).sc(0.7f).jit(6, 45) }), C({ PB(1764).c(1).sc(0.35f).jit(6) }) }), 0.75f),
        };
        v.push_back(t);
    }
    // 15 --------------------------------------------------------------- Inferno
    {
        Template t; t.id = "inferno"; t.name = "Inferno"; t.by = "Claude";
        t.desc = "Dark rock, glowing lava cracks, flames and embers";
        t.palette = { S(40, 15, 10, false, "Rock"), S(255, 112, 22, false, "Lava"), S(255, 222, 85, false, "Hot"),
                      S(255, 95, 20, true, "Glow"), S(205, 42, 12, false, "Edge") };
        t.hueVar = 6;
        t.features = {
            fill(0),
            glow(3, 1.4f, true, ZT2, 1, 1u << 2),
            outline(4, 2.f),
            inter(Pattern::Random, 0.3f, { C({ PB(1369).c(1).sc(0.6f).jit(5, 180).zo(3) }), C({ PB(1372).c(1).sc(0.6f).jit(5, 180).zo(3) }),
                                           C({ PB(1655).c(2).sc(0.5f).jit(5, 180).zo(3) }, 0.5f) }),
            withJ(surf(Side::Top, 0.3f, { C({ PB(1766).c(1).sc(1.1f, 2.f).at(0, 8).jit(3, 8), PB(1766).c(2).sc(0.7f, 1.2f).at(0, 4.8f).zo(7), PB(504).c(3).sc(0.6f).at(0, 6).z(ZB1, 0) }) }), 0.1f),
            surf(Side::Bottom, 0.4f, { C({ PB(1766).c(1).sc(1.f, 1.6f).at(0, 6.4f) }), C({ PB(1766).c(1).sc(0.8f, 1.2f).at(-5, 4.8f), PB(1766).c(2).sc(0.6f, 1.f).at(5, 4.f) }) }),
            glow(3, 1.1f),
            withChance(scatter(0.07f, 2, { C({ PB(1764).c(2).sc(0.3f).jit(8) }), C({ PB(1764).c(1).sc(0.4f).jit(8) }) }), 0.8f),
        };
        v.push_back(t);
    }
    // 16 --------------------------------------------------------------- Deep Ocean
    {
        Template t; t.id = "deep-ocean"; t.name = "Deep Ocean"; t.by = "Claude";
        t.desc = "Deep blue reef blocks, seaweed, coral and bubbles";
        t.palette = { S(10, 42, 92, false, "Deep"), S(60, 200, 230, false, "Aqua"), S(205, 245, 255, false, "Foam"),
                      S(40, 150, 255, true, "Glow"), S(255, 122, 100, false, "Coral"), S(42, 162, 122, false, "Seaweed") };
        t.features = {
            fill(0),
            glow(3, 0.8f, true),
            outline(1, 1.5f),
            withChance(band(0, 3, 2, 1.5f), 0.5f),
            surf(Side::Top, 0.65f, {
                C({ PB(1756).c(5).sc(0.8f, 1.f).at(0, 15).jit(4, 10) }),
                C({ PB(1756).c(5).sc(0.7f, 0.7f).at(-5, 10.5f).jit(3, 8), PB(1756).c(5).sc(0.7f, 1.2f).at(4, 18).jit(3, 8) }),
                C({ PB(123).c(4).sc(0.5f).at(0, 7.5f).flip() }, 0.6f),
                C({ PB(125).c(4).sc(0.5f).at(0, 7.5f).flip() }, 0.4f),
            }),
            inter(Pattern::Random, 0.2f, { C({ PB(51).c(2).sc(0.2f).jit(8).zo(3) }) }),
            withChance(scatter(0.1f, 3, { C({ PB(51).c(2).sc(0.22f).jit(8, 0, 0.4f) }), C({ PB(51).c(2).sc(0.14f).jit(8) }) }), 0.9f),
        };
        v.push_back(t);
    }
    // 17 --------------------------------------------------------------- Candy Pop
    {
        Template t; t.id = "candy-pop"; t.name = "Candy Pop"; t.by = "Claude";
        t.desc = "Pastel candy blocks, icing drips, sprinkles and hearts";
        t.palette = { S(255, 172, 212, false, "Candy"), S(255, 255, 255, false, "Icing"), S(150, 240, 200, false, "Mint"),
                      S(255, 80, 160, false, "Hot pink"), S(255, 240, 140, false, "Lemon") };
        t.swaps = { { 0, 2 } }; t.slotSwap = 0.3f; t.hueVar = 10;
        auto spr = [](float x, float y, float r, int c) { return sq(6, 1.8f).c(c).at(x, y).r(r).zo(4); };
        t.features = {
            fill(0),
            outline(3, 3.f),
            outline(1, 1.5f, 5.f, 4),
            surf(Side::Top, 1.f, { C({ PB(1893).c(1).at(0, -6).zo(7) }), C({ PB(1896).c(1).at(0, -6).zo(7) }), C({ PB(1897).c(1).at(0, -6).zo(7) }) }),
            inter(Pattern::Random, 0.35f, { C({ spr(-6, 2, 30, 2), spr(5, -4, -40, 4), spr(2, 7, 80, 3) }), C({ PB(52).c(3).sc(0.3f).zo(4) }, 0.5f) }, 2, 2, 1),
            corner(0.5f, TOPS, { C({ PB(50).c(3).sc(0.35f).at(-9, 5.2f), sq(1.2f, 7).c(2).r(20).at(-8, 11) }) }),
            withChance(scatter(0.05f, 2, { C({ PB(52).c(3).sc(0.3f).jit(6, 15) }), C({ PB(54).c(4).sc(0.3f).jit(6, 30) }) }), 0.7f),
        };
        v.push_back(t);
    }
    // 18 --------------------------------------------------------------- Haunted Night
    {
        Template t; t.id = "haunted"; t.name = "Haunted Night"; t.by = "Claude";
        t.desc = "Dark purple ruins, thorns, skulls, chains and ghost lights";
        t.palette = { S(30, 15, 40, false, "Dark"), S(232, 222, 200, false, "Bone"), S(120, 255, 150, true, "Ghost glow"),
                      S(120, 60, 162, false, "Edge"), S(62, 30, 72, false, "Thorn") };
        t.features = {
            fill(0),
            outline(3, 2.f),
            glow(2, 0.9f),
            surf(Side::Top, 0.4f, { C({ PB(123).c(4).sc(0.8f).at(0, 12).flip() }), C({ PB(126).c(4).sc(0.8f).at(0, 7.5f).flip() }),
                                    C({ PB(1596).c(1).sc(0.6f).at(0, 5).flip() }, 0.3f) }),
            withChance(surf(Side::Bottom, 0.3f, { C({ PB(106).c(3).sc(0.45f).at(0, 15) }), C({ PB(107).c(3).sc(0.45f).at(0, 15) }) }, 2, false), 0.8f),
            inter(Pattern::Random, 0.15f, { C({ PB(1597).c(1).sc(0.4f).v(0.6f).jit(5).zo(3) }, 0.4f), C({ PB(1653).c(3).sc(0.5f).jit(5, 180).zo(3) }) }),
            withChance(scatter(0.05f, 2, { C({ PB(1764).c(2).sc(0.45f).jit(8) }) }), 0.8f),
        };
        v.push_back(t);
    }
    // 19 --------------------------------------------------------------- Clockwork
    {
        Template t; t.id = "clockwork"; t.name = "Clockwork"; t.by = "Claude";
        t.desc = "Bronze machinery with spinning gears, rivets and pipes";
        t.palette = { S(152, 102, 52, false, "Bronze"), S(45, 30, 20, false, "Dark"), S(232, 182, 92, false, "Brass"),
                      S(192, 92, 52, false, "Copper"), S(255, 180, 80, true, "Glow") };
        t.features = {
            fill(1),
            outline(2, 2.5f),
            outline(3, 1.f, 5.f, 4),
            withChance(band(9, 3, 3, 5.f), 0.6f),
            inter(Pattern::Random, 0.25f, { C({ PB(87).c(0).sc(0.55f).anim().zo(3) }), C({ PB(97).c(2).sc(0.6f).anim().zo(3) }),
                                            C({ PB(86).c(0).sc(0.45f).anim().zo(3) }, 0.6f) }, 2, 2, 1),
            corner(0.7f, 0xF, { C({ PB(1764).c(2).sc(0.5f).at(-6, -6).zo(7) }) }),
            surf(Side::Top, 0.22f, { C({ PB(86).c(0).sc(0.6f).at(0, 0).anim().z(ZT1, 0) }) }, 3),
            withChance(surf(Side::Bottom, 0.3f, { C({ sq(6, 14).c(3).at(0, 7), sq(9, 3).c(2).at(0, 14).zo(7) }) }, 2, false), 0.8f),
            withChance(glow(4, 0.6f), 0.5f),
        };
        v.push_back(t);
    }
    // 20 --------------------------------------------------------------- Galaxy Dream
    {
        Template t; t.id = "galaxy"; t.name = "Galaxy Dream"; t.by = "Claude";
        t.desc = "Space blocks with nebula glow, stars, rings and planets";
        t.palette = { S(20, 10, 52, false, "Space"), S(140, 60, 255, true, "Nebula"), S(255, 255, 255, false, "Star"),
                      S(100, 222, 255, false, "Cyan"), S(255, 80, 200, true, "Pink nebula") };
        t.swaps = { { 1, 4 } }; t.slotSwap = 0.4f; t.hueVar = 12;
        t.features = {
            fill(0),
            glow(1, 1.3f, true),
            outline(3, 1.5f),
            glow(4, 0.9f),
            inter(Pattern::Random, 0.3f, { C({ PB(54).c(2).sc(0.2f).jit(8, 30).zo(3) }), C({ PB(1764).c(2).sc(0.22f).jit(9).zo(3) }) }),
            withChance(scatter(0.12f, 3, { C({ PB(54).c(2).sc(0.25f).jit(8, 30, 0.3f) }), C({ PB(51).c(3).sc(0.22f).jit(8) }, 0.5f),
                                           C({ PB(1764).c(2).sc(0.3f).jit(8) }) }), 0.9f),
            withChance(surf(Side::Top, 0.12f, { C({ PB(50).c(3).sc(0.4f).at(0, 16), PB(51).c(2).sc(0.65f, 0.22f).r(-15).at(0, 16).zo(7) }) }, 4), 0.7f),
        };
        v.push_back(t);
    }
    return v;
}

} // namespace

std::vector<Template> const& builtinTemplates() {
    static std::vector<Template> v = make();
    return v;
}

} // namespace deco

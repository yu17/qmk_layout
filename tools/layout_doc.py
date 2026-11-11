#!/usr/bin/env python3
"""Render docs/layout.html: every layer of the yu17 keymaps, read from keymap.c.

Usage: python3 tools/layout_doc.py [LEGACY_QMK_DIR]
(LEGACY_QMK_DIR, default ../qmk_firmware_legacy, supplies the CTRL key geometry.)
"""
import datetime, json, re, sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
LEGACY = Path(sys.argv[1]) if len(sys.argv) > 1 else REPO.parent / 'qmk_firmware_legacy'


def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', '', s, flags=re.S)
    return re.sub(r'//[^\n]*', '', s)


def parse(path):
    s = strip_comments(path.read_text()).replace('\\\n', ' ')
    defs = {m.group(1): m.group(2).strip() for m in re.finditer(r'#define\s+(\w+)\s+([^\n]+)', s)}
    s = s[s.index('keymaps[]'):]
    layers = []
    for m in re.finditer(r'\[(\w+)\]\s*=\s*(LAYOUT\w*)\(', s):
        i = j = m.end()
        depth = 1
        while depth:
            depth += (s[j] == '(') - (s[j] == ')')
            j += 1
        args, d, cur = [], 0, ''
        for c in s[i:j - 1]:
            if c == ',' and d == 0:
                args.append(cur.strip())
                cur = ''
                continue
            d += (c == '(') - (c == ')')
            cur += c
        if cur.strip():
            args.append(cur.strip())

        def resolve(a):
            for _ in range(5):
                if a not in defs:
                    break
                a = defs[a]
            return re.sub(r'\s+', '', a)

        layers.append({'id': m.group(1), 'keys': [resolve(a) for a in args]})
    return layers


km = REPO / 'keyboards'
data = {
    'generated': datetime.date.today().isoformat(),
    'planck': parse(km / 'planck/keymaps/yu17/keymap.c'),
    'geonix': parse(km / 'geonix41/geonix41/keymaps/yu17/keymap.c'),
    'ctrl': parse(km / 'massdrop/ctrl/keymaps/yu17/keymap.c'),
}
kj = json.loads((LEGACY / 'keyboards/massdrop/ctrl/keyboard.json').read_text())
layout = kj['layouts']['LAYOUT']['layout']
data['ctrl_geom'] = [{'x': k['x'], 'y': k['y'], 'w': k.get('w', 1)} for k in layout]
for name in ('planck', 'geonix'):
    assert all(len(l['keys']) == 48 for l in data[name]), name
assert all(len(l['keys']) == len(layout) for l in data['ctrl'])

tpl = (REPO / 'tools/layout_template.html').read_text()
out = REPO / 'docs/layout.html'
out.parent.mkdir(exist_ok=True)
out.write_text(tpl.replace('/*DATA*/null', json.dumps(data, separators=(',', ':'))))
print(f'wrote {out.relative_to(REPO)}')

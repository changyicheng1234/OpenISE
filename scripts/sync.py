#!/usr/bin/env python3
from __future__ import annotations

import argparse
import datetime as dt
import json
import logging
import re
import shutil
import subprocess
from pathlib import Path
from typing import Any
LOGGER = logging.getLogger("sync")
ROOT_INDEX = Path("content/doc/_index.md")
FILES_JSON = Path("data/files.json")
RAW_BASE_URL = (
    "https://raw.githubusercontent.com/Timlin15/sysu-ise-course-materials/main"
)
GHFAST_PREFIX = "https://ghfast.top/"
GITHUB_BLOB_BASE = "https://github.com"
SKIP_NAMES = {".git", ".github", ".gitignore", ".DS_Store", "__pycache__"}
DEFAULT_MAJOR_KEYWORDS = ("智能科学与技术", "智慧交通")
SPECIAL_TOP_LEVEL_DIRS = ("大一公共课程", "往届课程")
TEMP_ROOT_DOC_FILES = ("全部课程复习策略整理.md",)
SEMESTER_PATTERN = re.compile(
    r"(大一|大二|大三|大四|大五|秋|春|上|下|学期|semester|term)", re.IGNORECASE
)
SORT_PREFIX_PATTERN = re.compile(r"^\d{1,3}[-_]+")
YEAR_WEIGHT_MAP = {"大一": 1, "大二": 2, "大三": 3, "大四": 4, "大五": 5}
FILETREE_SHORTCODE = """{{- $path := .Get "path" -}}
{{- $node := index site.Data.files $path -}}

<div class="course-filetree hx:my-8">
  <style>
    .course-filetree {
      --ft-border: rgba(15, 23, 42, 0.08);
      --ft-border-strong: rgba(15, 23, 42, 0.12);
      --ft-panel: rgba(255, 255, 255, 0.82);
      --ft-panel-muted: rgba(248, 250, 252, 0.9);
      --ft-text: #0f172a;
      --ft-text-muted: #64748b;
      --ft-accent: #2563eb;
      --ft-accent-soft: rgba(37, 99, 235, 0.08);
      --ft-shadow: 0 20px 45px -30px rgba(15, 23, 42, 0.45);
      color: var(--ft-text);
    }
    html.dark .course-filetree {
      --ft-border: rgba(148, 163, 184, 0.16);
      --ft-border-strong: rgba(148, 163, 184, 0.24);
      --ft-panel: rgba(10, 15, 28, 0.82);
      --ft-panel-muted: rgba(15, 23, 42, 0.92);
      --ft-text: rgba(241, 245, 249, 0.96);
      --ft-text-muted: rgba(148, 163, 184, 0.92);
      --ft-accent: #60a5fa;
      --ft-accent-soft: rgba(96, 165, 250, 0.14);
      --ft-shadow: 0 24px 55px -34px rgba(2, 6, 23, 0.85);
    }
    .course-filetree-shell {
      overflow: hidden;
      border: 1px solid var(--ft-border);
      border-radius: 24px;
      background: var(--ft-panel);
      box-shadow: var(--ft-shadow);
      backdrop-filter: blur(18px);
    }
    .course-filetree-head {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 1rem;
      padding: 1rem 1.25rem;
      border-bottom: 1px solid var(--ft-border);
      background: linear-gradient(180deg, var(--ft-panel-muted), transparent);
    }
    .course-filetree-title {
      display: flex;
      align-items: center;
      min-width: 0;
    }
    .course-filetree-title strong {
      display: block;
      font-size: 0.98rem;
      font-weight: 700;
      line-height: 1.2;
    }
    .course-filetree-title span {
      display: block;
      margin-top: 0.18rem;
      color: var(--ft-text-muted);
      font-size: 0.83rem;
      word-break: break-all;
    }
    .course-filetree-grid,
    .course-filetree-row,
    .course-filetree-summary {
      display: grid;
      grid-template-columns: minmax(0, 1fr) 124px 120px 88px;
      gap: 0.75rem;
      align-items: center;
    }
    .course-filetree-grid {
      padding: 0.85rem 1.25rem;
      color: var(--ft-text-muted);
      font-size: 0.76rem;
      font-weight: 700;
      letter-spacing: 0.04em;
      text-transform: uppercase;
      border-bottom: 1px solid var(--ft-border);
    }
    .course-filetree-body {
      padding: 0.35rem 0.5rem 0.65rem;
    }
    .course-filetree-empty {
      padding: 1rem 1.1rem;
      margin: 0.45rem;
      border: 1px dashed var(--ft-border-strong);
      border-radius: 18px;
      color: var(--ft-text-muted);
      background: var(--ft-panel-muted);
    }
    .course-filetree-folder {
      margin: 0.28rem 0;
      border: 1px solid var(--ft-border);
      border-radius: 18px;
      overflow: hidden;
      background: color-mix(in srgb, var(--ft-panel-muted) 75%, transparent);
    }
    .course-filetree-folder[open] {
      border-color: var(--ft-border-strong);
    }
    .course-filetree-summary,
    .course-filetree-row {
      padding: 0.78rem 0.95rem;
    }
    .course-filetree-summary {
      cursor: pointer;
      list-style: none;
      transition: background-color 0.2s ease, transform 0.2s ease;
    }
    .course-filetree-summary::-webkit-details-marker {
      display: none;
    }
    .course-filetree-summary:hover,
    .course-filetree-row:hover {
      background: color-mix(in srgb, var(--ft-accent-soft) 50%, transparent);
    }
    .course-filetree-name {
      display: flex;
      align-items: center;
      gap: 0.78rem;
      min-width: 0;
      font-weight: 600;
    }
    .course-filetree-icon,
    .course-filetree-action {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      flex-shrink: 0;
    }
    .course-filetree-icon {
      width: 2rem;
      height: 2rem;
      border-radius: 0.75rem;
      color: var(--ft-accent);
      background: var(--ft-accent-soft);
    }
    .course-filetree-folder .course-filetree-icon {
      color: #d97706;
      background: rgba(245, 158, 11, 0.14);
    }
    html.dark .course-filetree-folder .course-filetree-icon {
      color: #fbbf24;
      background: rgba(251, 191, 36, 0.14);
    }
    .course-filetree-name-text {
      min-width: 0;
    }
    .course-filetree-name-text strong,
    .course-filetree-name-text a {
      display: block;
      min-width: 0;
      white-space: nowrap;
      overflow: hidden;
      text-overflow: ellipsis;
      font-size: 0.95rem;
      color: inherit;
      text-decoration: none;
    }
    .course-filetree-name-text span {
      display: block;
      margin-top: 0.18rem;
      color: var(--ft-text-muted);
      font-size: 0.78rem;
    }
    .course-filetree-meta {
      color: var(--ft-text-muted);
      font-size: 0.84rem;
      white-space: nowrap;
    }
    .course-filetree-actions {
      display: flex;
      justify-content: flex-end;
      gap: 0.35rem;
    }
    .course-filetree-action {
      width: 2rem;
      height: 2rem;
      border-radius: 999px;
      border: 1px solid var(--ft-border);
      color: var(--ft-text-muted);
      background: transparent;
      transition: all 0.2s ease;
    }
    .course-filetree-action:hover {
      color: var(--ft-accent);
      border-color: color-mix(in srgb, var(--ft-accent) 28%, var(--ft-border));
      background: var(--ft-accent-soft);
    }
    .course-filetree-chevron {
      transition: transform 0.2s ease;
    }
    .course-filetree-folder[open] > .course-filetree-summary .course-filetree-chevron {
      transform: rotate(90deg);
    }
    .course-filetree-children {
      padding: 0 0.45rem 0.45rem;
      border-top: 1px solid var(--ft-border);
    }
    @media (max-width: 900px) {
      .course-filetree-grid {
        display: none;
      }
      .course-filetree-row,
      .course-filetree-summary {
        grid-template-columns: minmax(0, 1fr) auto;
        grid-template-areas:
          "name actions"
          "meta meta";
      }
      .course-filetree-name {
        grid-area: name;
      }
      .course-filetree-actions {
        grid-area: actions;
      }
      .course-filetree-summary > :nth-child(2),
      .course-filetree-summary > :nth-child(3),
      .course-filetree-row > :nth-child(2),
      .course-filetree-row > :nth-child(3) {
        grid-area: meta;
        padding-left: calc(var(--depth, 0) * 16px + 2.78rem);
      }
      .course-filetree-summary > :nth-child(3),
      .course-filetree-row > :nth-child(3) {
        margin-top: -0.35rem;
      }
    }
  </style>

    <div class="course-filetree-shell">
      <div class="course-filetree-head">
        <div class="course-filetree-title">
        <div>
          <strong>课程资料</strong>
          <span>{{ $path }}</span>
        </div>
      </div>
    </div>

    <div class="course-filetree-grid" aria-hidden="true">
      <div>文件名</div>
      <div>文件大小</div>
      <div>最后修改日期</div>
      <div style="text-align:right;">操作</div>
    </div>

    <div class="course-filetree-body">
      {{- if $node -}}
        {{- partial "filetree-render.html" (dict "node" $node "depth" 0 "path" $path) -}}
      {{- else -}}
        <div class="course-filetree-empty">暂无文件</div>
      {{- end -}}
    </div>
  </div>
</div>
"""
FILETREE_PARTIAL = """{{- $node := .node -}}
{{- $depth := .depth | default 0 -}}
{{- $path := .path | default "" -}}
{{- $names := slice -}}
{{- range $name, $_ := $node -}}
  {{- $names = $names | append $name -}}
{{- end -}}
{{- range sort $names -}}
  {{- $name := . -}}
  {{- $entry := index $node $name -}}
  {{- $padding := mul $depth 18 -}}
  {{- if eq $entry._type "folder" -}}
    <details class="course-filetree-folder" {{ if lt $depth 1 }}open{{ end }}>
      <summary class="course-filetree-summary" style="--depth: {{ $depth }};">
        <div class="course-filetree-name" style="padding-left: {{ $padding }}px;">
          <span class="course-filetree-icon" aria-hidden="true">{{ partial "filetree-icon.html" (dict "kind" "folder" "name" $name) }}</span>
          <span class="course-filetree-name-text">
            <strong>{{ $name }}</strong>
            <span>文件夹</span>
          </span>
        </div>
        <div class="course-filetree-meta">文件夹</div>
        <div class="course-filetree-meta">-</div>
        <div class="course-filetree-actions">
          <span class="course-filetree-action course-filetree-chevron" aria-hidden="true">
            <svg width="16" height="16" viewBox="0 0 24 24" fill="none">
              <path d="m9 6 6 6-6 6" stroke="currentColor" stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round"/>
            </svg>
          </span>
        </div>
      </summary>
      <div class="course-filetree-children">
        {{- partial "filetree-render.html" (dict "node" $entry._children "depth" (add $depth 1) "path" (printf "%s/%s" $path $name)) -}}
      </div>
    </details>
  {{- else if eq $entry._type "file" -}}
    <div class="course-filetree-row" style="--depth: {{ $depth }};">
      <div class="course-filetree-name" style="padding-left: {{ $padding }}px;">
        <span class="course-filetree-icon" aria-hidden="true">{{ partial "filetree-icon.html" (dict "kind" "file" "name" $entry.name) }}</span>
        <span class="course-filetree-name-text">
          <a href="{{ $entry.url }}" target="_blank" rel="noreferrer">{{ $entry.name }}</a>
          <span>{{ path.Ext $entry.name | strings.TrimPrefix "." | upper }}</span>
        </span>
      </div>
      <div class="course-filetree-meta">{{ $entry.sizeHuman }}</div>
      <div class="course-filetree-meta">{{ $entry.mtime }}</div>
      <div class="course-filetree-actions">
        <a class="course-filetree-action" href="{{ $entry.mirrorUrl }}" target="_blank" rel="noreferrer" aria-label="加速下载 {{ $entry.name }}" title="加速下载">
          <svg width="15" height="15" viewBox="0 0 24 24" fill="none">
            <path d="M12 4v10" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/>
            <path d="m8 10 4 4 4-4" stroke="currentColor" stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round"/>
            <path d="M5 19h14" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/>
          </svg>
        </a>
        <a class="course-filetree-action" href="{{ $entry.repoUrl }}" target="_blank" rel="noreferrer" aria-label="跳转GitHub {{ $entry.name }}" title="跳转GitHub">
          <svg width="15" height="15" viewBox="0 0 24 24" fill="currentColor" aria-hidden="true">
            <path d="M12 .5C5.65.5.5 5.7.5 12.1c0 5.12 3.3 9.47 7.88 11 .58.11.79-.26.79-.57 0-.28-.01-1.2-.02-2.17-3.2.71-3.88-1.38-3.88-1.38-.52-1.35-1.28-1.7-1.28-1.7-1.05-.73.08-.72.08-.72 1.16.08 1.77 1.21 1.77 1.21 1.03 1.8 2.7 1.28 3.36.98.1-.76.4-1.28.73-1.58-2.55-.3-5.23-1.3-5.23-5.78 0-1.28.45-2.32 1.2-3.14-.12-.3-.52-1.52.11-3.17 0 0 .98-.32 3.2 1.2a10.9 10.9 0 0 1 5.82 0c2.22-1.52 3.2-1.2 3.2-1.2.63 1.65.23 2.87.11 3.17.75.82 1.2 1.86 1.2 3.14 0 4.49-2.69 5.48-5.25 5.77.41.36.78 1.08.78 2.19 0 1.58-.01 2.85-.01 3.24 0 .31.21.69.8.57A11.62 11.62 0 0 0 23.5 12.1C23.5 5.7 18.35.5 12 .5Z"/>
          </svg>
        </a>
      </div>
    </div>
  {{- end -}}
{{- end -}}
"""
FILETREE_ICON_PARTIAL = """{{- $kind := .kind -}}
{{- $name := lower (.name | default "") -}}
{{- $ext := path.Ext $name -}}

{{- if eq $kind "folder" -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <path d="M3.5 7.5A2.5 2.5 0 0 1 6 5h3.3c.5 0 1 .2 1.4.55l1.3 1.3c.19.19.44.3.7.3H18a2.5 2.5 0 0 1 2.5 2.5v6.82A2.5 2.5 0 0 1 18 19H6a2.5 2.5 0 0 1-2.5-2.5V7.5Z" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
  </svg>
{{- else if or (eq $ext ".pdf") (eq $ext ".ps") -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <path d="M7 3.75h6.25L18.25 8v12.25A1.75 1.75 0 0 1 16.5 22h-9A1.75 1.75 0 0 1 5.75 20.25v-14.75A1.75 1.75 0 0 1 7.5 3.75Z" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
    <path d="M13 3.75V8h5.25" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
    <path d="M8.25 15.5h7.5M8.25 18.25h5" stroke="currentColor" stroke-width="1.7" stroke-linecap="round"/>
  </svg>
{{- else if or (eq $ext ".doc") (eq $ext ".docx") (eq $ext ".pages") -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <path d="M7 3.75h6.25L18.25 8v12.25A1.75 1.75 0 0 1 16.5 22h-9A1.75 1.75 0 0 1 5.75 20.25v-14.75A1.75 1.75 0 0 1 7.5 3.75Z" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
    <path d="M13 3.75V8h5.25" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
    <path d="M8.4 16.1c0-1.5.88-2.4 2.24-2.4 1.38 0 2.24.9 2.24 2.4 0 1.5-.86 2.4-2.24 2.4-1.36 0-2.24-.9-2.24-2.4Zm6.05 1.95V14.1h1.35v3.95h-1.35Z" stroke="currentColor" stroke-width="1.35" stroke-linecap="round" stroke-linejoin="round"/>
  </svg>
{{- else if or (eq $ext ".ppt") (eq $ext ".pptx") (eq $ext ".key") -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <path d="M7 3.75h6.25L18.25 8v12.25A1.75 1.75 0 0 1 16.5 22h-9A1.75 1.75 0 0 1 5.75 20.25v-14.75A1.75 1.75 0 0 1 7.5 3.75Z" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
    <path d="M13 3.75V8h5.25" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
    <path d="M8.5 18.25v-4.5h2.1c.96 0 1.65.65 1.65 1.58s-.69 1.57-1.65 1.57H8.5Zm6.1-4.5h1.1l.95 4.5h-1.15l-.18-.98h-1.25l-.18.98h-1.15l.86-4.5Zm.1 2.7h.8l-.39-1.92h-.03l-.38 1.92Z" fill="currentColor"/>
  </svg>
{{- else if or (eq $ext ".zip") (eq $ext ".rar") (eq $ext ".7z") (eq $ext ".tar") (eq $ext ".gz") -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <rect x="5" y="3.75" width="14" height="16.5" rx="2.5" stroke="currentColor" stroke-width="1.7"/>
    <path d="M12 5.75v2.5M12 10v1.75M12 13.5v1.75M10.25 18h3.5" stroke="currentColor" stroke-width="1.7" stroke-linecap="round"/>
  </svg>
{{- else if or (eq $ext ".png") (eq $ext ".jpg") (eq $ext ".jpeg") (eq $ext ".gif") (eq $ext ".webp") (eq $ext ".svg") -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <rect x="4.75" y="5.25" width="14.5" height="13.5" rx="2" stroke="currentColor" stroke-width="1.7"/>
    <circle cx="9" cy="10" r="1.5" fill="currentColor"/>
    <path d="m7 16 3.2-3.2a1 1 0 0 1 1.4 0L13 14l1.7-1.7a1 1 0 0 1 1.41.01L18 14.25" stroke="currentColor" stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round"/>
  </svg>
{{- else if or (eq $ext ".py") (eq $ext ".cpp") (eq $ext ".c") (eq $ext ".h") (eq $ext ".java") (eq $ext ".js") (eq $ext ".ts") (eq $ext ".go") (eq $ext ".rs") (eq $ext ".m") (eq $ext ".xml") (eq $ext ".json") -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <path d="M8.5 8.25 5.75 12l2.75 3.75M15.5 8.25 18.25 12l-2.75 3.75M13.5 6.75 10.5 17.25" stroke="currentColor" stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round"/>
  </svg>
{{- else -}}
  <svg width="18" height="18" viewBox="0 0 24 24" fill="none">
    <path d="M7 3.75h6.25L18.25 8v12.25A1.75 1.75 0 0 1 16.5 22h-9A1.75 1.75 0 0 1 5.75 20.25v-14.75A1.75 1.75 0 0 1 7.5 3.75Z" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
    <path d="M13 3.75V8h5.25" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>
  </svg>
{{- end -}}
"""
HEAD_END_PARTIAL = """<style>
  @media (min-width: 768px) {
    .hextra-sidebar-container.hx\\:md\\:hidden.hx\\:xl\\:block {
      display: flex !important;
    }
  }
</style>
"""
CUSTOM_CSS = """@media (min-width: 768px) {
  .hextra-sidebar-container.hx\\:md\\:hidden.hx\\:xl\\:block {
    display: flex !important;
  }
}

/* Shields badges are inline labels, not standalone content images. */
.content img[src*="img.shields.io/"] {
  display: inline-block;
  width: auto;
  margin: 0 0.25rem 0 0;
  border-radius: 0;
  vertical-align: middle;
}
"""
DOC_LIST_TEMPLATE = """{{ define "main" }}
  <div class='hx:mx-auto hx:flex hextra-max-page-width'>
    {{ partial "sidebar.html" (dict "context" .) }}
    {{ partial "toc.html" . }}
    <article class="hx:w-full hx:break-words hx:flex hx:min-h-[calc(100vh-var(--navbar-height))] hx:min-w-0 hx:justify-center hx:pb-8 hx:pr-[calc(env(safe-area-inset-right)-1.5rem)]">
      <main id="content" class="hx:w-full hx:min-w-0 hextra-max-content-width hx:px-6 hx:pt-4 hx:md:px-12">
        {{ partial "breadcrumb.html" (dict "page" . "enable" true) }}
        <div class="content">
          {{ if .Title }}
          <div class="hx:flex hx:flex-col hx:sm:flex-row hx:items-start hx:sm:items-center hx:sm:justify-between hx:gap-4 hx:mb-4">
            <h1 class="hx:mb-0">{{ .Title }}</h1>
            {{ partial "components/page-context-menu" . }}
          </div>
          {{ end }}
          {{ .Content }}
        </div>
        {{ partial "components/last-updated.html" . }}
        {{- if (site.Params.page.displayPagination | default true) -}}
          {{- partial "components/pager.html" . -}}
        {{- end -}}
        {{ partial "components/comments.html" . }}
      </main>
    </article>
  </div>
{{ end }}
"""
DOC_SINGLE_TEMPLATE = DOC_LIST_TEMPLATE


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate Hugo content and files.json from course materials."
    )
    parser.add_argument(
        "--source",
        type=Path,
        default=Path.cwd(),
        help="Path to the sysu-ise-course-materials repository.",
    )
    parser.add_argument(
        "--target",
        type=Path,
        required=True,
        help="Path to the openise repository.",
    )
    parser.add_argument(
        "--base-url",
        default=None,
        help="GitHub raw base URL used in files.json.",
    )
    parser.add_argument(
        "--repo",
        default=None,
        help="Source GitHub repository in owner/name format, used to build raw URLs.",
    )
    parser.add_argument(
        "--branch",
        default="main",
        help="Source branch name used to build raw URLs.",
    )
    parser.add_argument(
        "--major-keyword",
        action="append",
        dest="major_keywords",
        help=(
            "Allowed top-level major keyword. Repeat this flag to allow multiple majors. "
            f"Defaults to: {', '.join(DEFAULT_MAJOR_KEYWORDS)}"
        ),
    )
    return parser.parse_args()


def configure_logging() -> None:
    logging.basicConfig(level=logging.INFO, format="[%(levelname)s] %(message)s")


def should_skip(path: Path, source_root: Path) -> bool:
    relative_parts = path.relative_to(source_root).parts
    return any(part.startswith(".") or part in SKIP_NAMES for part in relative_parts)


def list_child_dirs(directory: Path, source_root: Path) -> list[Path]:
    children = [
        child
        for child in directory.iterdir()
        if child.is_dir() and not should_skip(child, source_root)
    ]
    return sorted(children, key=lambda item: item.name)


def list_child_files(directory: Path, source_root: Path) -> list[Path]:
    children = [
        child
        for child in directory.iterdir()
        if child.is_file() and not should_skip(child, source_root)
    ]
    return sorted(children, key=lambda item: item.name)


def is_semester_dir(directory: Path) -> bool:
    return bool(SEMESTER_PATTERN.search(directory.name))


def is_allowed_major_dir(directory: Path, major_keywords: tuple[str, ...]) -> bool:
    return directory.name in SPECIAL_TOP_LEVEL_DIRS or any(
        keyword in directory.name for keyword in major_keywords
    )


def strip_sort_prefix(name: str) -> str:
    return SORT_PREFIX_PATTERN.sub("", name)


def extract_sort_weight(name: str) -> int | None:
    match = SORT_PREFIX_PATTERN.match(name)
    if match:
        prefix = match.group(0).rstrip("-_")
        try:
            return int(prefix) + 1
        except ValueError:
            return None

    normalized = strip_sort_prefix(name)
    year_weight = next(
        (weight for label, weight in YEAR_WEIGHT_MAP.items() if label in normalized),
        None,
    )
    if year_weight is None:
        return None

    if "上" in normalized or "秋" in normalized:
        term_weight = 1
    elif "下" in normalized or "春" in normalized:
        term_weight = 2
    else:
        term_weight = 0

    return year_weight * 10 + term_weight


def parse_frontmatter_and_body(text: str) -> tuple[dict[str, str], str]:
    stripped = text.lstrip("\ufeff")
    if not stripped.startswith("---\n"):
        return {}, stripped

    end_marker = "\n---\n"
    end = stripped.find(end_marker, 4)
    if end == -1:
        return {}, stripped

    frontmatter_block = stripped[4:end]
    body = stripped[end + len(end_marker) :].lstrip("\n")
    data: dict[str, str] = {}

    for raw_line in frontmatter_block.splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#") or ":" not in line:
            continue
        key, value = line.split(":", 1)
        key = key.strip()
        value = value.strip().strip("'\"")
        if key:
            data[key] = value

    return data, body


def extract_first_h1(text: str) -> str | None:
    _, body = parse_frontmatter_and_body(text)
    for line in body.splitlines():
        stripped = line.strip()
        if stripped.startswith("# "):
            return stripped[2:].strip()
    return None


def load_readme(directory: Path) -> tuple[dict[str, str], str] | None:
    readme_path = directory / "README.md"
    if not readme_path.exists():
        return None
    text = readme_path.read_text(encoding="utf-8")
    frontmatter, body = parse_frontmatter_and_body(text)
    return frontmatter, body


def resolve_title(directory: Path) -> str:
    loaded = load_readme(directory)
    if not loaded:
        title = directory.name
        if is_semester_dir(directory):
            return strip_sort_prefix(title)
        return title

    frontmatter, body = loaded
    title = frontmatter.get("title")
    if title:
        return title

    heading = extract_first_h1(body)
    if heading:
        return heading

    if is_semester_dir(directory):
        return strip_sort_prefix(directory.name)
    return directory.name


def ensure_frontmatter(title: str, body: str) -> str:
    normalized_body = body.strip()
    if normalized_body.startswith("---\n"):
        return normalized_body.rstrip() + "\n"
    if normalized_body:
        return f'---\ntitle: "{title}"\n---\n\n{normalized_body}\n'
    return f'---\ntitle: "{title}"\n---\n'


def normalize_heading_text(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip()


def strip_duplicate_leading_h1(title: str, body: str) -> str:
    lines = body.splitlines()
    index = 0
    while index < len(lines) and not lines[index].strip():
        index += 1

    if index >= len(lines):
        return body

    first_line = lines[index].strip()
    if not first_line.startswith("# "):
        return body

    index += 1
    while index < len(lines) and not lines[index].strip():
        index += 1
    return "\n".join(lines[index:]).lstrip("\n")


def render_placeholder(title: str) -> str:
    return f'---\ntitle: "{title}"\n---\n'


def render_frontmatter(frontmatter: dict[str, str]) -> str:
    lines = ["---"]
    for key, value in frontmatter.items():
        if isinstance(value, bool):
            serialized = "true" if value else "false"
        elif isinstance(value, (int, float)):
            serialized = str(value)
        else:
            escaped = str(value).replace('"', '\\"')
            serialized = f'"{escaped}"'
        lines.append(f"{key}: {serialized}")
    lines.append("---")
    return "\n".join(lines) + "\n"


def write_markdown(
    target_file: Path,
    title: str,
    body: str,
    extra: str | None = None,
    extra_frontmatter: dict[str, str] | None = None,
) -> None:
    target_file.parent.mkdir(parents=True, exist_ok=True)
    extra_frontmatter = extra_frontmatter or {}
    normalized_body = strip_duplicate_leading_h1(title, body)
    content = (
        ensure_frontmatter(title, normalized_body)
        if normalized_body
        else render_placeholder(title)
    )
    if extra_frontmatter:
        merged_frontmatter = {"title": title, **extra_frontmatter}
        normalized_body = normalized_body.strip()
        content = render_frontmatter(merged_frontmatter)
        if normalized_body:
            content += "\n" + normalized_body + "\n"
    if extra:
        content = content.rstrip() + "\n\n" + extra.rstrip() + "\n"
    target_file.write_text(content, encoding="utf-8")
    LOGGER.info("Wrote %s", target_file)


def render_cards(item_dirs: list[Path]) -> str:
    lines = ["{{< cards >}}"]
    sorted_items = sorted(
        item_dirs,
        key=lambda item: (
            extract_sort_weight(item.name) if extract_sort_weight(item.name) is not None else 10_000,
            item.name,
        ),
    )
    for item_dir in sorted_items:
        title = resolve_title(item_dir).replace('"', '\\"')
        slug = item_dir.name.replace('"', '\\"')
        lines.append(f'  {{{{< card link="{slug}" title="{title}" >}}}}')
    lines.append("{{< /cards >}}")
    return "\n".join(lines)


def human_size(size_bytes: int) -> str:
    size = float(size_bytes)
    units = ["B", "KB", "MB", "GB", "TB"]
    for unit in units:
        if size < 1024 or unit == units[-1]:
            if unit == "B":
                return f"{int(size)} B"
            return f"{size:.1f} {unit}"
        size /= 1024
    return f"{size_bytes} B"


def resolve_repository(source_root: Path, explicit_repo: str | None) -> str:
    if explicit_repo:
        return explicit_repo

    try:
        result = subprocess.run(
            ["git", "-C", str(source_root), "remote", "get-url", "origin"],
            check=True,
            capture_output=True,
            text=True,
        )
    except (subprocess.CalledProcessError, FileNotFoundError):
        LOGGER.warning("Failed to detect git remote, falling back to default raw base URL.")
        return ""

    remote = result.stdout.strip()
    match = re.search(r"github\.com[:/](?P<repo>[^/]+/[^/.]+)(?:\.git)?$", remote)
    if match:
        return match.group("repo")

    LOGGER.warning("Failed to parse GitHub repository from remote: %s", remote)
    return ""


def resolve_base_url(
    source_root: Path, explicit_base_url: str | None, explicit_repo: str | None, branch: str
) -> str:
    if explicit_base_url:
        return explicit_base_url

    repo = resolve_repository(source_root, explicit_repo)
    if repo:
        return f"https://raw.githubusercontent.com/{repo}/{branch}"

    return RAW_BASE_URL


def build_file_tree(
    directory: Path,
    source_root: Path,
    base_url: str,
    github_repo: str,
    branch: str,
) -> dict[str, Any]:
    tree: dict[str, Any] = {}
    for child in sorted(directory.iterdir(), key=lambda item: (item.is_file(), item.name)):
        if should_skip(child, source_root):
            continue
        if child.is_file():
            if child.name == "README.md" or child.suffix.lower() == ".md":
                continue
            relative_path = child.relative_to(source_root).as_posix()
            stat = child.stat()
            raw_url = f"{base_url.rstrip('/')}/{relative_path}"
            tree[child.name] = {
                "_type": "file",
                "name": child.name,
                "size": stat.st_size,
                "sizeHuman": human_size(stat.st_size),
                "mtime": dt.datetime.fromtimestamp(stat.st_mtime).strftime("%Y/%m/%d"),
                "url": raw_url,
                "mirrorUrl": f"{GHFAST_PREFIX}{raw_url}",
                "repoUrl": f"{GITHUB_BLOB_BASE}/{github_repo}/blob/{branch}/{relative_path}",
            }
        elif child.is_dir():
            subtree = build_file_tree(child, source_root, base_url, github_repo, branch)
            tree[child.name] = {"_type": "folder", "_children": subtree}
    return tree


def find_course_dirs(semester_dir: Path, source_root: Path) -> list[Path]:
    return list_child_dirs(semester_dir, source_root)


def write_course_page(
    content_root: Path,
    target_path: Path,
    target_url: str,
    course_dir: Path,
    source_root: Path,
) -> str:
    course_title = resolve_title(course_dir)
    course_readme = load_readme(course_dir)
    course_body = course_readme[1] if course_readme else ""
    course_rel = course_dir.relative_to(source_root).as_posix()
    course_weight = extract_sort_weight(course_dir.name)
    resources_block = (
        "## 资源下载\n\n"
        f'{{{{< filetree path="{course_rel}" >}}}}'
    )
    course_frontmatter: dict[str, Any] = {
        "url": target_url
    }
    if course_weight is not None:
        course_frontmatter["weight"] = course_weight
    write_markdown(
        content_root / target_path,
        course_title,
        course_body,
        resources_block,
        extra_frontmatter=course_frontmatter,
    )
    return course_rel


def sync_temp_root_docs(source_root: Path, content_root: Path) -> None:
    for filename in TEMP_ROOT_DOC_FILES:
        source_file = source_root / filename
        if not source_file.exists():
            LOGGER.info("Temporary root doc not found, skipping: %s", source_file)
            continue

        text = source_file.read_text(encoding="utf-8")
        frontmatter, body = parse_frontmatter_and_body(text)
        title = (
            frontmatter.get("title")
            or extract_first_h1(body)
            or source_file.stem
        )
        write_markdown(
            content_root / source_file.name,
            title,
            body,
            extra_frontmatter={"url": f"/doc/{source_file.stem}/"},
        )


def preserve_root_index(target_root: Path) -> str | None:
    root_index = target_root / ROOT_INDEX
    if not root_index.exists():
        LOGGER.info("No existing %s to preserve.", root_index)
        return None
    LOGGER.info("Preserving existing %s", root_index)
    return root_index.read_text(encoding="utf-8")


def restore_root_index(target_root: Path, preserved_root_index: str | None) -> None:
    if preserved_root_index is None:
        return
    root_index = target_root / ROOT_INDEX
    root_index.parent.mkdir(parents=True, exist_ok=True)
    root_index.write_text(preserved_root_index, encoding="utf-8")
    LOGGER.info("Restored %s", root_index)


def generate_specialized_content(
    source_root: Path,
    target_root: Path,
    base_url: str,
    github_repo: str,
    branch: str,
    major_keywords: tuple[str, ...],
) -> dict[str, Any]:
    files_index: dict[str, Any] = {}
    content_root = target_root / "content/doc"

    for major_dir in list_child_dirs(source_root, source_root):
        if not is_allowed_major_dir(major_dir, major_keywords):
            LOGGER.info("Skipping top-level directory outside major filter: %s", major_dir.name)
            continue

        semester_dirs = [
            item for item in list_child_dirs(major_dir, source_root) if is_semester_dir(item)
        ]
        major_title = resolve_title(major_dir)
        major_readme = load_readme(major_dir)
        major_body = major_readme[1] if major_readme else ""
        major_weight = extract_sort_weight(major_dir.name)
        major_frontmatter: dict[str, Any] = {"url": f"/doc/{major_dir.name}/"}
        if major_weight is not None:
            major_frontmatter["weight"] = major_weight
        major_cards_block = render_cards(semester_dirs) if semester_dirs else None
        write_markdown(
            content_root / major_dir.name / "_index.md",
            major_title,
            major_body,
            major_cards_block,
            extra_frontmatter=major_frontmatter,
        )

        if semester_dirs:
            for semester_dir in semester_dirs:
                course_dirs = find_course_dirs(semester_dir, source_root)
                semester_title = resolve_title(semester_dir)
                semester_readme = load_readme(semester_dir)
                semester_body = semester_readme[1] if semester_readme else ""
                semester_weight = extract_sort_weight(semester_dir.name)
                cards_block = render_cards(course_dirs) if course_dirs else None
                semester_frontmatter: dict[str, Any] = {
                    "url": f"/doc/{major_dir.name}/{semester_dir.name}/"
                }
                if semester_weight is not None:
                    semester_frontmatter["weight"] = semester_weight
                write_markdown(
                    content_root / major_dir.name / semester_dir.name / "_index.md",
                    semester_title,
                    semester_body,
                    cards_block,
                    extra_frontmatter=semester_frontmatter,
                )

                for course_dir in course_dirs:
                    course_rel = write_course_page(
                        content_root,
                        Path(major_dir.name) / semester_dir.name / f"{course_dir.name}.md",
                        f"/doc/{major_dir.name}/{semester_dir.name}/{course_dir.name}/",
                        course_dir,
                        source_root,
                    )
                    files_index[course_rel] = build_file_tree(
                        course_dir, source_root, base_url, github_repo, branch
                    )
            continue

        course_dirs = find_course_dirs(major_dir, source_root)
        if not course_dirs:
            LOGGER.info("Skipping non-standard top-level directory: %s", major_dir.name)
            continue

        cards_block = render_cards(course_dirs)
        write_markdown(
            content_root / major_dir.name / "_index.md",
            major_title,
            major_body,
            cards_block,
            extra_frontmatter=major_frontmatter,
        )
        for course_dir in course_dirs:
            course_rel = write_course_page(
                content_root,
                Path(major_dir.name) / f"{course_dir.name}.md",
                f"/doc/{major_dir.name}/{course_dir.name}/",
                course_dir,
                source_root,
            )
            files_index[course_rel] = build_file_tree(
                course_dir, source_root, base_url, github_repo, branch
            )

    return files_index


def write_files_json(target_root: Path, files_index: dict[str, Any]) -> None:
    data_dir = target_root / "data"
    data_dir.mkdir(parents=True, exist_ok=True)
    target_file = target_root / FILES_JSON
    target_file.write_text(
        json.dumps(files_index, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )
    LOGGER.info("Wrote %s", target_file)


def write_openise_support_files(target_root: Path) -> None:
    shortcode_path = target_root / "layouts/shortcodes/filetree.html"
    partial_path = target_root / "layouts/partials/filetree-render.html"
    icon_partial_path = target_root / "layouts/partials/filetree-icon.html"
    head_end_path = target_root / "layouts/_partials/custom/head-end.html"
    sidebar_partial_path = target_root / "layouts/_partials/sidebar.html"
    custom_css_path = target_root / "assets/css/custom.css"
    doc_list_path = target_root / "layouts/doc/list.html"
    doc_single_path = target_root / "layouts/doc/single.html"
    template_root = Path(__file__).resolve().parent / "templates"
    sidebar_template_path = template_root / "sidebar.html"

    shortcode_path.parent.mkdir(parents=True, exist_ok=True)
    partial_path.parent.mkdir(parents=True, exist_ok=True)
    icon_partial_path.parent.mkdir(parents=True, exist_ok=True)
    head_end_path.parent.mkdir(parents=True, exist_ok=True)
    sidebar_partial_path.parent.mkdir(parents=True, exist_ok=True)
    custom_css_path.parent.mkdir(parents=True, exist_ok=True)
    doc_list_path.parent.mkdir(parents=True, exist_ok=True)
    doc_single_path.parent.mkdir(parents=True, exist_ok=True)

    shortcode_path.write_text(FILETREE_SHORTCODE, encoding="utf-8")
    partial_path.write_text(FILETREE_PARTIAL, encoding="utf-8")
    icon_partial_path.write_text(FILETREE_ICON_PARTIAL, encoding="utf-8")
    if not head_end_path.exists():
        head_end_path.write_text(HEAD_END_PARTIAL, encoding="utf-8")
        LOGGER.info("Wrote %s", head_end_path)
    else:
        LOGGER.info("Preserved existing %s", head_end_path)
    custom_css_path.write_text(CUSTOM_CSS, encoding="utf-8")
    doc_list_path.write_text(DOC_LIST_TEMPLATE, encoding="utf-8")
    doc_single_path.write_text(DOC_SINGLE_TEMPLATE, encoding="utf-8")
    if sidebar_template_path.exists():
        shutil.copyfile(sidebar_template_path, sidebar_partial_path)
        LOGGER.info("Wrote %s", sidebar_partial_path)
    else:
        LOGGER.warning("Sidebar template not found: %s", sidebar_template_path)

    LOGGER.info("Wrote %s", shortcode_path)
    LOGGER.info("Wrote %s", partial_path)
    LOGGER.info("Wrote %s", icon_partial_path)
    LOGGER.info("Wrote %s", head_end_path)
    LOGGER.info("Wrote %s", custom_css_path)
    LOGGER.info("Wrote %s", doc_list_path)
    LOGGER.info("Wrote %s", doc_single_path)


def ensure_doc_menu_page_ref(target_root: Path) -> None:
    hugo_config = target_root / "hugo.yaml"
    if not hugo_config.exists():
        LOGGER.warning("Skipping hugo.yaml update because the file does not exist.")
        return

    content = hugo_config.read_text(encoding="utf-8")
    updated = content.replace("pageRef: /docs", "pageRef: /doc")
    if updated != content:
        hugo_config.write_text(updated, encoding="utf-8")
        LOGGER.info("Updated %s to use /doc", hugo_config)
    else:
        LOGGER.info("No pageRef update needed in %s", hugo_config)


def validate_paths(source_root: Path, target_root: Path) -> None:
    if not source_root.exists():
        raise FileNotFoundError(f"Source repository does not exist: {source_root}")
    if not target_root.exists():
        raise FileNotFoundError(f"Target repository does not exist: {target_root}")


def main() -> int:
    args = parse_args()
    configure_logging()

    source_root = args.source.resolve()
    target_root = args.target.resolve()
    major_keywords = tuple(args.major_keywords or DEFAULT_MAJOR_KEYWORDS)
    github_repo = resolve_repository(source_root, args.repo) or "Timlin15/sysu-ise-course-materials"
    base_url = resolve_base_url(source_root, args.base_url, args.repo, args.branch)

    LOGGER.info("Source: %s", source_root)
    LOGGER.info("Target: %s", target_root)
    LOGGER.info("Allowed majors: %s", ", ".join(major_keywords))
    LOGGER.info("GitHub repo: %s", github_repo)
    LOGGER.info("Raw base URL: %s", base_url)
    validate_paths(source_root, target_root)

    content_root = target_root / "content/doc"
    preserved_root_index = preserve_root_index(target_root)
    if content_root.exists():
        shutil.rmtree(content_root)
        LOGGER.info("Removed %s", content_root)
    content_root.mkdir(parents=True, exist_ok=True)
    restore_root_index(target_root, preserved_root_index)
    sync_temp_root_docs(source_root, content_root)
    files_index = generate_specialized_content(
        source_root, target_root, base_url, github_repo, args.branch, major_keywords
    )
    write_files_json(target_root, files_index)
    write_openise_support_files(target_root)
    ensure_doc_menu_page_ref(target_root)

    LOGGER.info("Sync generation completed successfully.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

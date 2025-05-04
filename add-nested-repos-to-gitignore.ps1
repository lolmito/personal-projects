# File to update
$gitignorePath = ".gitignore"

# Get all directories named ".git" and their parent directories
$nestedRepos = Get-ChildItem -Recurse -Directory -Force -Filter ".git" | ForEach-Object {
    $_.Parent.FullName
}

# Get relative paths and format for .gitignore
$repoRoot = (Get-Location).Path
$ignoreEntries = $nestedRepos | ForEach-Object {
    $relative = $_.Replace($repoRoot, "").TrimStart('\')
    "/" + ($relative -replace '\\','/') + "/"
}

# Read existing .gitignore entries (if any)
if (Test-Path $gitignorePath) {
    $existingEntries = Get-Content $gitignorePath
} else {
    $existingEntries = @()
}

# Merge and remove duplicates
$allEntries = $existingEntries + $ignoreEntries
$uniqueEntries = $allEntries | Sort-Object -Unique

# Save back to .gitignore
$uniqueEntries | Set-Content $gitignorePath -Encoding UTF8

Write-Host ".gitignore updated with nested Git repo folders."

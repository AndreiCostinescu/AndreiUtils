# Branch Protection Configuration

This file documents the recommended GitHub branch protection settings for the
`main` branch.  These cannot be committed to the repo — you must apply them
manually in **Settings → Branches → Add branch protection rule**.

---

## Required status checks for `main`

*Note: Checks must run at least once before they appear in the UI list.*

Enable **"Require status checks to pass before merging"** and add:

| Check name                                                                 | Provided by                               |
|-----------------------------------------------------------------------------|-------------------------------------------|
| `build (gcc, <feature-set>)` and `build (clang, <feature-set>)` — one pair per `feature_set` entry in `ci.yml`'s build matrix (19 feature sets x 2 compilers = 38 checks; see the comment at the top of `ci.yml` for how that matrix was derived from `CMakeLists.txt`) | `ci.yml`                                  |
| `lint`                                                                     | `lint.yml`                                |
| `DCO`                                                                      | [GitHub App](https://github.com/apps/dco) |

Add all 38 `build (...)` checks individually in the branch protection UI (it only lists checks that
have run at least once, so push once first) — there's no wildcard/glob support for required checks.

Also enable:
- ✅ **Require branches to be up to date before merging**
- ✅ **Require signed commits** ← enforces GPG/SSH signing at the Git level
- ✅ **Do not allow bypassing the above settings** (even for admins, recommended)

Then go to repository settings, in the Commits section, and enable **"Require contributors to sign off on web-based commits"**

---

## Signed commits vs DCO — what is each?

| Mechanism | What it proves | How |
|---|---|---|
| **DCO** (`Signed-off-by:`) | Contributor certifies they have the right to submit the code (legal) | `git commit -s` |
| **GPG / SSH commit signing** | The commit was actually made by the key owner (identity) | `git commit -S` (capital S) |

Both are enforced in this repo:
- DCO is checked by the [GitHub App](https://github.com/apps/dco) on every PR.
- GPG/SSH signing is enforced by the GitHub branch protection rule **"Require signed commits"**.

Contributors need to set up both:
```bash
# Sign-off (DCO) — always pass -s
git commit -s -m "feat: my change"

# GPG signing — configure once, then use -S or set commit.gpgsign=true
git config --global commit.gpgsign true
git config --global user.signingkey <YOUR_KEY_ID>
```

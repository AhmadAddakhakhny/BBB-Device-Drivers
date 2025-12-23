# Guide to Linux kernel development

## Course legends
![alt text](imgs/image.png)

---
### How often would a kernel release take?
> 10 to 11 weeks and it's time based not feature based.

### Show linux development life cycle?
1. After releasing a mainline release, Linus Torvalds opens PR window for 2 weeks
2. publishes a release cycle **rc1**, and would take around a week for bug fixing and regressions
3. then would do number of **rcX** upto 7 or 8 weeks until feels confident of the release quality
![alt text](imgs/image2.png)

### Active kernel releases?
1. Release Candidate (RC) (pre-release)
> Linux x.y-rc: as (x) is a major relase number and (y) a minor number. and (rc) used to denote that this release used for integration and regressions testing afther the merge window closes.  
2. Stable
> Stable releases are bug-fix-only releases. so after Linus releases a mainline kernel, it moves into stable mode. (it gets periodically patches once a week or as-needed basis)  
3. Longterm (LTS)
> Longterm releases are stable releases selected for long-term maintenance to provide critical bug fixes for older kenel trees.
---

### Kernel Trees - what are they?
1. the mainline kernel tree: managed by **Linus Torvalds**
2. the stable tree: managed by **Greg Kroah-Hartman**
3. the linux-next tree: managed by **Stephen Rothwell**
```
 This is an integration tree. Code from many subsystem trees is periodically pulled into this tree and then released for integration testing. Pulling changes from various trees catches merge conflicts (if any) between trees.
```
> Use case
```
One of Shuah's first actions as a maintainer was to request that her tree be added to linux-next. After she commits patches to her tree, Shuah waits 3 to 7 days before sending a pull request to Linus, giving enough time to find problems and regressions, if any. Patches applied to a tree that will be added to linux-next are only for the next merge window, including during the merge window. Patches for the next release may be added to linux-next after the merge window has closed, and the rc1 candidate has been released by Linus.
```
---
### Sybsystem Maintainers
> Each major subsystem has its own tree and designated maintainer(s).
![alt text](imgs/image3.png)
---

## Patches
### What is a patch?
> $ git format-patch -1 --pretty=fuller **COMMIT-SHA**  
> it gives you a compelete information about the change  
1. Commit ID: SHA-1 hash
2. Commit header: subsystem:sub-driver: do_func() return path OR subsystem/sub-driver: do_func() return path  
2.1. usbip:usbip_host: cleanup do_rebind() return path  
2.2. usbip/usbip_host: cleanup do_rebind() return path  
3. Commit log: answers in details the what and why?
4. Author: name, email address as configured in .gitconfig.  
4.1. P.S. the Signed-off-by should match the address from which you send patches.  
5. AuthorDate: time-date of a commit (comes from dev-computer time system)
6. Commit: commiter's name and email.
7. CommitDate: the time-date of applying this patch to the subsystem tree.
8. signed-off-by: Developers certify the patch to be their original work or have the right to pass it on as an open source patch.
---

## Tags
### 1. Acked-by
```
This tag is often used by the maintainer of the affected code when that maintainer neither contributed to nor forwarded the patch. For example, Shuah maintains the usbip driver and she uses the Acked-by tag to ask the USB maintainer to pick patches sent by other developers.

✅ The reviewer agrees with the technical correctness and intent of the patch
✅ The reviewer believes it is appropriate for the subsystem
❌ The reviewer is not the author
❌ The reviewer is not necessarily the maintainer
❌ It does not guarantee testing, only agreement
```

### 2. Reviewed-by
```
This tag indicates that the patch has been reviewed by the person named in the tag.

✅ The reviewer read the patch in detail
✅ The reviewer checked logic, correctness, and design
✅ The reviewer agrees the patch is ready to be merged
❌ It does not imply testing (that’s Tested-by)
❌ It does not imply authorship or responsibility for merging
```

### 3. Reported-by
```
This tag gives credit to people who find bugs and report them.

✅ Identifies the original bug reporter
✅ Can be a developer, tester, or end user
❌ The reporter did not necessarily provide a fix
❌ It does not imply review or approval
```

### 4. Tested-by
```
This tag indicates that the patch has been tested by the person named in the tag.

✅ The patch was compiled and/or executed
✅ The test did not expose failures or regressions
❌ It does not imply code review
❌ It does not imply approval of design or correctness
```

### 5. Suggested-by
```
This tag is used to give credit for the patch idea to the person named in the tag.

✅ Credits conceptual contribution
✅ The suggestion may be high-level or detailed
❌ The suggester is not the author
❌ It does not imply review, testing, or approval
```

### 6. Fixes
```
This tag indicates that the patch fixes an issue in a previous commit referenced by its Commit ID. This tag allows us to track where the bug originated.

✅ Links a bug fix to the exact faulty commit
✅ Helps maintainers and tools understand where the bug came from
✅ Enables automatic stable backporting
```
---
## Patch email subject line convention
```
Let's find out how to prefix patch email subject lines. The [PATCH] prefix is used to indicate that the email consists of a patch. [PATCH RFC] or [RFC PATCH] indicates the author is requesting comments on the patch. RFC stands for "Request For Comments". [PATCH v4] is used to indicate that the patch is the 4th version of this specific change that is being submitted. It is not unusual for a patch to go through a few revisions before it gets accepted. This is an artifact of collaborative development. The goal is to get the code right and not rush it in.
```

## Patch Version History
```
Including the patch version history when sending a re-worked patch is required. The patch revision history on what changed between the current and the previous versions is added “---” and “start of the diff” in the patch file. Any text that is added here gets thrown away and will not be included in the commit when it is merged into the source tree. Including information that helps with reviews and doesn't add value to the commit log here is good practice. Please check mailing lists to get a feel for what information gets added here.

Do not send new patch versions as a reply to a previous version. Start a new thread for each version of a patch. An example description of what changed is: "Changes since v3: Added null check for <variable name> as suggested by <name>." You can see a patch example with version history for the v2 version.
https://patchwork.kernel.org/project/linux-kselftest/patch/20190926224014.28910-1-skhan@linuxfoundation.org/
```

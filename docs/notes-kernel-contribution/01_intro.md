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

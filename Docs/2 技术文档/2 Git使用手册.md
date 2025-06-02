# Git的学习之旅

## 逻辑图

![asd](https://www.runoob.com/wp-content/uploads/2015/02/git-command.jpg)

## 在本地创建仓库

### 创建一个新的目录&进入仓库目录

```bash
mkdir my-project
cd my-project
```

### 让当前目录变为仓库——初始化

```bash
git init
```

### 💡让上述两个步骤简化

```bash
mkdir newrepo  # 创建 newrepo 目录
cd newrepo     # 进入 newrepo 目录
git init       # 在 newrepo 目录中初始化 Git 仓库
```

👇如下

```bash
git init newrepo
```

### echo指令——创建文件

```bash
echo '内容' > name.py
```

刚创建的文件是**未被追踪的** ，可用**git status** 查看,如下👇

```bash
$ echo '张一哲-2025年2月27日15点40分' > readme.txt
$ git status

On branch main

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        readme.txt

nothing added to commit but untracked files present (use "git add" to track)
```

### rm指令——删除文件

👇删除名为 .git文件夹

```bash
rm -rf .git
```

## git add指令——追踪 [🔗]([git add 命令 | 菜鸟教程 (runoob.com)](https://www.runoob.com/git/git-add.html))

**git add** 命令可将该文件的修改添加到**暂存区**,然后等待提交（**git commit**）。

```bash
#添加当前目录下的所有文件到暂存区：

git add .
```

如下👇 此时**已经被追踪**

```bash
$ git add readme.txt
$ git status

On branch main

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   readme.txt
```

### git commit指令——本地 [🔗]([git commit 命令 | 菜鸟教程 (runoob.com)](https://www.runoob.com/git/git-commit.html))

git commit 命令将暂存区内容**提交到本地仓库**中。

```bash
git commit -m [备注信息]
```

提交暂存区的**指定文件**到仓库区：

```bash
git commit [file1] [file2] ... -m [message]
```

如下👇

```bash
git commit -m [这是一条备注信息]
[main (root-commit) 4b617f5] [这是一条备注信息]
 1 file changed, 1 insertion(+)
 create mode 100644 readme.txt
```

## **在本地删除 Git 仓库**

1. **进入你的项目目录**（确保你在正确的目录下）：
   
   ```bash
   cd /你的项目路径
   ```

2. **删除 `.git` 目录**（这个目录包含了所有 Git 相关的数据）：
   
   ```bash
   rm -rf .git
   ```
   
   ⚠ **注意**：执行此命令后，Git 不再跟踪该目录，所有提交记录、分支信息都会丢失，无法恢复！请确认你真的不需要这个仓库了。

3. **验证删除是否成功**：
   
   ```bash
   git status
   ```
   
   如果返回：
   
   ```
   fatal: not a git repository (or any of the parent directories): .git
   ```
   
   说明 Git 仓库已成功删除。

## 分支管理

<img src="https://static.jyshare.com/images/svg/git-brance.svg" alt="Git 分支示意图" width="400">

👆这个是HTML语言嵌入SVG图 img标签，下面这行空白是md语言渲染的👇

![Git 分支示意图](https://static.jyshare.com/images/svg/git-brance.svg)

这个👆是md语言，只不过这里渲染不出来，在浏览器里可以看见

## 🎯 **常见分支模型**

1. `main`（主分支）：**稳定**的生产环境代码
2. `dev`（开发分支）：日常开发，**定期**合并到 `main`
3. `feature-xxx`（功能分支）：每个**新功能**一个独立分支，完成后合并到 `dev`
4. `bugfix-xxx`（修复分支）：修复**特定 bug**，完成后合并到 `dev` 或 `main`

---

## 🚀 **常用 Git 分支操作**

### **1️⃣ 创建新分支**

```sh
git branch feature-golf-tracking
```

✅ 这样就创建了一个 **feature-golf-tracking** 分支（但还在 `main` 分支）。

---

### **2️⃣ 切换到新分支**

```sh
git checkout feature-golf-tracking
```

📌 现在你就在 `feature-golf-tracking` 分支，可以在上面开发新功能。

**快捷方式**（创建 + 切换）：

```sh
git checkout -b feature-golf-tracking
```

---

### **3️⃣ 查看当前分支**

```sh
git branch
```

🌟 你会看到类似这样：

```
  main
* feature-golf-tracking
```

`*` 号表示你当前所在的分支。

---

### **4️⃣ 在新分支上开发，并提交**

假设你修改了 `main.py`，然后：

```sh
git add main.py
git commit -m "实现高尔夫球检测功能"
```

---

### **5️⃣ 切回 `main` 分支**

```sh
git checkout main
```

---

### **6️⃣ 合并分支**

假设 `feature-golf-tracking` 分支的开发完成，你想合并到 `main`：

```sh
git merge feature-golf-tracking
```

🔹 这样 `feature-golf-tracking` 的代码就合并到 `main` 了！

---

### **7️⃣ 删除分支**

如果 `feature-golf-tracking` 分支已经合并，想删除它：

```sh
git branch -d feature-golf-tracking
```

如果分支未合并，但你仍想强制删除：

```sh
git branch -D feature-golf-tracking
```

---

## 🔥 **进阶：推送/拉取 远程分支**

如果你想把本地 `feature-golf-tracking` 推送到远程 GitHub：

```sh
git push -u origin feature-golf-tracking
```

如果你想删除远程分支：

```sh
git push origin --delete feature-golf-tracking
```

如果你在远程修改了一些文件，想要拉取到本地同步

```bash
git pull origin branch_name
```

---

## ✅ **总结**

- **创建分支**：`git branch 分支名`
- **切换分支**：`git checkout 分支名`
- **创建并切换**：`git checkout -b 分支名`
- **合并分支**：`git merge 分支名`
- **删除分支**：`git branch -d 分支名`
- **推送分支**：`git push origin 分支名`
- **拉取分支**：`git pull origin 分支名`

---

1. **（可选）删除 `feature_1` 分支**：
   
   ```sh
   git branch -d feature_1
   ```

2. **（可选）推送到远程仓库**：
   
   ```sh
   git push origin main
   ```

## 📝.gitignore 文件

在仓库根目录下创建一个 .gitignore 文件，这其实是一个txt文件，在里面输入想要忽略的编译文件，例如👇

```bash
*.pyc
```

## 📝.gitattributes文件

在仓库根目录下创建，添加以下内容：

```bash
readme.md merge=ours
```

这条规则的意思是：当合并时遇到`readme.md`文件，总是保留‌**当前分支**‌的版本（即"ours"策略），忽略其他分支的修改。

### 扩展配置（可选）

如果你有其他需要独立保留的文件（比如`CHANGELOG.md`），可以继续在`.gitattributes`中添加规则：

```bash
gitattributesCopy Code

readme.md merge=ours CHANGELOG.md merge=ours
```

这样每个分支的指定文件都会保持独立。

### git log——查看历史[🔗]([Git 查看提交历史 | 菜鸟教程 (runoob.com)](https://www.runoob.com/git/git-commit-history.html))

```bash
git log
```

## 🌏Github分支管理

### **Create a merge commit**（创建合并提交）：

- 这是最直接的合并方式。当你选择这个选项时，GitHub会**创建一个新的合并**提交来将你的分支合并到目标分支。这种方式**保留了项目的历史记录**，因为它不会重写提交历史。即使两个分支的历史完全平行，也会通过一个新的合并提交将它们连接起来。

### **Squash and merge**（压缩并合并）：

- 此选项允许你将一系列的提交压缩成一个单独的提交，然后再将其合并到目标分支。这对于保持项目历史清晰特别有用，尤其是当特性分支包含**多个小的、增量更新**的时候。它有助于**避免历史记录过于碎片**化，使得历史更加整洁和易于理解。

### **Rebase and merge**（变基并合并）：

- 使用这个选项，GitHub会首先将**目标分支的所有更改变基到你的分支**上，然后进行一次快速向前合并。这意味着，如果你的分支是从目标分支的最新提交开始的话，最终的结果是没有合并提交的干净历史。这要求你的分支上的工作基于目标分支的最新状态，因此在执行之前通常需要先同步最新的变更。

每种方法的选择取决于团队的工作流程和个人偏好，以及如何管理项目的Git历史。例如，**如果希望保持一个非常线性和简单的历史记录**，可能会倾向于使用“**Rebase and merge**”。而**如果更重视每个功能分支**的**独立性**及其对项目的贡献，则可能更喜欢“**Create a merge commit”或者“Squash and merge”**。 

## GitHub加速⏩

编辑路径上host文件

```bash
C:\Windows\System32\drivers\etc
```

CMD

```bash
ipconfig/flushdns
```

# 最新更新：2025年3月14日🗓

有待后续更新

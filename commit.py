import configparser
import argparse
from enum import Enum
from plumbum import local, LocalPath

class ColorCode(Enum):
    FAIL = "\033[91m"
    RESET_STYLE = "\033[0m"

class ResultCode(Enum):
	SUCCESS = "Success"
	ERROR = "Error"

def is_git_repo_root(path: LocalPath) -> bool:
	git = local["git"]
	code, repo_root, _ = git["rev-parse", "--show-toplevel"].with_cwd(path).run(retcode=(128,0))
	if code:
		return False
	return repo_root.strip() == str(path).strip()

def clean_repo(path: LocalPath) -> None:
	rm = local["rm"]
	for item in path:
		if item.name != ".git":
			rm["-rf", item]()

def copy_contents(src: LocalPath, dest: LocalPath) -> None:
	git = local["git"]
	rsync = local["rsync"]
	for item in git["ls-tree", "-r", "HEAD", "--name-only", "."].with_cwd(src)().split():
		rsync["-R", item, dest].with_cwd(src)()

def pull_if_possible(repo: LocalPath) -> None:
	git = local["git"]
	retcode, out, err = git["pull"].with_cwd(repo).run(retcode=None)
	if retcode:
		print(f"ignoring pull problems, err=[{err}], out=[{out}]")

def push(repo: LocalPath) -> None:
	git = local["git"]
	git["push"].with_cwd(repo)()

def commit(repo: LocalPath, message: str) -> None:
	git = local["git"]
	git["add", "."].with_cwd(repo)()
	git["commit", "-m", message].with_cwd(repo)()

def submit_changes(message: str, solution_path_str: str, intranet_repo_path_str: str) -> ResultCode:
	intranet_repo_path = local.path(intranet_repo_path_str)
	solution_path = local.path(solution_path_str)
	if not intranet_repo_path.exists():
		print(f"{ColorCode.FAIL.value}ERROR: folder '{intranet_repo_path}' doesn't exist{ColorCode.RESET_STYLE.value}")
		return ResultCode.ERROR
	if not is_git_repo_root(intranet_repo_path):
		print(f"{ColorCode.FAIL.value}ERROR: folder '{intranet_repo_path}' isn't a root of git repo{ColorCode.RESET_STYLE.value}")
		return ResultCode.ERROR
	if not solution_path.exists():
		print(f"{ColorCode.FAIL.value}ERROR: folder '{solution_path}' doesn't exist{ColorCode.RESET_STYLE.value}")
		return ResultCode.ERROR
	pull_if_possible(intranet_repo_path)
	clean_repo(intranet_repo_path)
	copy_contents(solution_path, intranet_repo_path)
	commit(intranet_repo_path, message)
	push(intranet_repo_path)
	return ResultCode.SUCCESS

def main():
    own_dir = local.cwd

    config = configparser.ConfigParser()
    config.read([own_dir / "example.ini", own_dir / "config.ini"])
    solution_path_str = config["DEFAULT"]["SolutionPath"]
    intranet_repo_path_str = config["DEFAULT"]["IntranetRepositoryPath"]

    parser = argparse.ArgumentParser()
    parser.add_argument("message", help="commit message for intranet repo")
    args = parser.parse_args()

    submit_changes(args.message, solution_path_str, intranet_repo_path_str)

if __name__ == "__main__":
	main()

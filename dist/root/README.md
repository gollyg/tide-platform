# %%PROJECT%%
Drupal 8 implementation of Content API for VIC.GOV.AU

[![CircleCI](https://circleci.com/gh/dpc-sdp/%%PROJECT%%.svg?style=shield&circle-token=619001ceda795d221a96315242e2782f621612d4)](https://circleci.com/gh/dpc-sdp/%%PROJECT%%)
![Release](https://img.shields.io/github/release/dpc-sdp/%%PROJECT%%.svg)

## Prerequisites
1. Make sure that you have latest versions of all required software installed:
  - [Docker](https://www.docker.com/)
  - [Pygmy](https://docs.amazee.io/local_docker_development/pygmy.html)
  - [Ahoy](https://github.com/ahoy-cli/ahoy)
2. Make sure that all local web development services are shut down (`apache/nginx`, `mysql`, `MAMP` etc).

## Local environment setup
3. `./dev-tools.sh`
4. `pygmy up`
5. `ahoy build`

Local URL -- http://%%PROJECT%%.docker.amazee.io/

## Available `ahoy` commands
Run each command as `ahoy <command>`.
```
 build                Build or rebuild project.
   clean                Remove all build files.
   clean-full           Remove all development files.
   cli                  Start a shell inside CLI container or run a command.
   composer-merge       Merge composer files.
   deploy               Deploy or re-deploy a branch in Bay.
   doctor               Identify problems with current stack.
   down                 Stop Docker containers and remove container, images, volumes and networks.
   drush                Run drush commands in the CLI service container.
   flush-redis          Flush Redis cache.
   info                 Print information about this project.
   install-dev          Install dependencies.
   install-site         Install site.
   lint                 Lint code.
   login                Login to a website.
   logs                 Show Docker logs.
   pull                 Pull latest docker images.
   restart              Restart all stopped and running Docker containers.
   start                Start existing Docker containers.
   stop                 Stop running Docker containers.
   test-behat           Run Behat tests.
   up                   Build and start Docker containers.
```

## SSHing into CLI container
`ahoy cli`

## Running a command in CLI container
`ahoy cli ls /app`

## Mailhog.

Mailhog is included with `pygmy` and is available @ http://mailhog.docker.amazee.io/
Documentation for mailhog is available of the project page -- https://github.com/mailhog/MailHog

## Stage file proxy.

Stage File Proxy is enabled on all non production environments so files are automatically downloaded directly from prod on demand.

## Adding Drupal modules
Modules needs to be added in 2 steps:
1. Require module code installation (through composer).
2. Enable module during site installation.

### Step 1. Adding contrib modules
`composer require drupal/module_name`
or for specific versions
`composer require drupal/module_name:1.2`

OR

### Step 1. Adding modules as local packages
1. Add local package information to root `composer.json`:

```
    "repositories": {
        "dpc-sdp/tide_page": {
            "type": "path",
            "url": "dpc-sdp/tide_page"
        },
    }
```
2. Assess if package is required for distribution (Tide) or site (content.vic.gov.au) and add to relevant `composer.json`:
  - for distribution - `dpc-sdp/tide/composer.json`
  - for site - `composer.json`
3. To make sure that Composer triggers dependency tree rebuild, run `ahoy clean`.
4. Run `composer update --lock`. This will install all dependencies and update root `composer.lock` file with newly added module.

### Step 2. Enable module
1. Assess if module is a part of distribution or site-specific and add to appropriate `info.yml` file:
  - for distribution - `dpc-sdp/tide/tide.info.yml`
  - for site - `docroot/modules/custom/vicgovau_core/vicgovau_core.info.yml`

If module is a dev-only module (required to be enabled for development only),
use `vicgovau_core_install()` in `docroot/modules/custom/vicgovau_core/vicgovau_core.install` to enable it programmatically. This is required as we are using site install and not storing exported configuration.

## Adding patches for drupal modules
1. Add `title` and `url` to patch on drupal.org to the `patches` array in `extra` section in `composer.json`.

```
    "extra": {
        "patches": {
            "drupal/core": {
                "Contextual links should not be added inside another link - https://www.drupal.org/node/2898875": "https://www.drupal.org/files/issues/contextual_links_should-2898875-3.patch"
            }
        }
    }
```

2. `composer update --lock`

## Coding standards
PHP and JS code linting uses [PHP_CodeSniffer](https://github.com/squizlabs/PHP_CodeSniffer) with Drupal rules from [Coder](https://www.drupal.org/project/coder) module and additional local overrides in `phpcs.xml.dist` and `.eslintrc`.

## Behat tests
Behat configuration uses multiple extensions:
- [Drupal Behat Extension](https://github.com/jhedstrom/drupalextension) - Drupal integration layer. Allows to work with Drupal API from within step definitions.
- [Behat Screenshot Extension](https://github.com/integratedexperts/behat-screenshot) - Behat extension and a step definition to create HTML and image screenshots on demand or test fail.
- [Behat Progress Fail Output Extension](https://github.com/integratedexperts/behat-format-progress-fail) - Behat output formatter to show progress as TAP and fail messages inline. Useful to get feedback about failed tests while continuing test run.
- `VicgovauDrupalContext` - Site-specific Drupal context with custom step definitions.
- `VicgovauMinkContext` - Site-specific Mink context with custom step definitions.

Generic Behat tests should be written against the test entities from the Tide Test module. If a new test entity (node, block, etc.) is added to the Tide Test module, the relevant permissions must be also granted to Approver and Editor via the hook `tide_test_entity_bundle_create()`.

### Run tests locally:
- Run Behat tests: `ahoy test-behat`
    - Run specific test feature: `ahoy test-behat tests/behat/features/homepage.feature`
    - Run specific test tag: `ahoy test-behat -- --tags=wip`

Read more information in [the wiki page](https://digital-engagement.atlassian.net/wiki/spaces/SDP/pages/134906009/Behat+testing).

## Automated builds (Continuous Integration)
In software engineering, continuous integration (CI) is the practice of merging all developer working copies to a shared mainline several times a day.
Before feature changes can be merged into a shared mainline, a complete build must run and pass all tests on CI server.

This project uses [Circle CI](https://circleci.com/) as CI server: it imports production backups into fully built codebase and runs code linting and tests. When tests pass, a deployment process is triggered for nominated branches (usually, `master` and `develop`).

Add [skip ci] to the commit subject to skip CI build. Useful for documentation changes.

### SSH
Circle CI supports SSHing into the build for 30 minutes after the build is finished. SSH can be enabled either during the build run or when the build is started with SSH support.

### Cache
Circle CI supports caching between builds. The cache takes care of saving the state of your dependencies between builds, therefore making the builds run faster.
Each branch of your project will have a separate cache. If it is the very first build for a branch, the cache from the default branch on GitHub (normally `master`) will be used. If there is no cache for master, the cache from other branches will be used.
If the build has inconsistent results (build fails in CI but passes locally), try to re-running the build without cache by clicking 'Rebuild without cache' button.

### Test artifacts
Test artifacts (screenshots etc.) are available under 'Artifacts' tab in Circle CI UI.

## Debugging

### PHP application from browser
1. Trigger xDebug from web browser (using one of the browser extensions) so that PHPStorm recognises the server `content-vicgovau.docker.amazee.io` and configures the path mapping. Alternatively, you can create the server in PHPStorm Settings.
  * Make sure `serverName` to be `content-vicgovau.docker.amazee.io`

### PHP scripts
1. Make sure `xdebug.sh` is executable.
2. SSH into CLI container: `docker-compose exec cli bash`
3. Run your PHP script: `xdebug.sh path/to/script`.
    * Example running a single Behat test: `xdebug.sh vendor/bin/behat path/to/test.feature`

### Drush commands
3. To debug `drush` commands:
    * SSH to CLI container: `docker-compose exec cli bash`
        + `cd docroot`
        + `./xdebug.sh ../vendor/bin/drush <DRUSH_COMMAND>`
            - Example: `./xdebug.sh ../vendor/bin/drush updb -y`

### DB connection details
Run `ahoy info` to get the port number.

```
  Host:     127.0.0.1
  Username: drupal
  Password: drupal
  Database: drupal
  Port:     <get from "ahoy info">
```

### Pre deployment database backups

An automatic backup of the production database is taken before any new deployment.
This is currently stored in the private files directory and overridden on each deployment.

#### Restoring a backup

1. Access production `cli` container on Bay.
2. Take a backup of failed deployment db for debugging.
  `drush sql-dump --gzip --result-file=/app/docroot/sites/default/files/private/failed_deployment.sql`
3. Import pre-deployment backup.
  `gzip -cd /app/docroot/sites/default/files/private/pre_deploy_backup.sql.gz | drush sqlc`
4. Clear cache.
  `drush cr`

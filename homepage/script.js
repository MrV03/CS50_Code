document.addEventListener('DOMContentLoaded', function () {
    // Animated role change
    const roleElement = document.getElementById('role');
    const roles = ['Student', 'Developer', 'Open to Work'];
    let roleIndex = 0;

    function changeRole() {
        roleElement.textContent = roles[roleIndex];

        setTimeout(() => {
            roleIndex = (roleIndex + 1) % roles.length;
            changeRole();
        }, 1500);
    }

    changeRole(); // Start role change

    // Welcome message
    const welcomeText = document.getElementById('welcome-text');
    const welcomeMessages = ['Welcome to my portfolio!', 'Explore and learn more about me.'];

    let messageIndex = 0;

    function changeWelcomeMessage() {
        welcomeText.textContent = welcomeMessages[messageIndex];

        setTimeout(() => {
            messageIndex = (messageIndex + 1) % welcomeMessages.length;
            changeWelcomeMessage();
        }, 3000);
    }

    // Start welcome message change immediately
    changeWelcomeMessage();

    // Ensure welcome message persists even when refreshing
    document.addEventListener('visibilitychange', function () {
        if (document.visibilityState === 'visible') {
            changeWelcomeMessage();
        }
    });
});

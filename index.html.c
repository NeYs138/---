<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Пермское Казино - Блекджек</title>
    <style>
        :root {
            --bg-color: #0f172a;
            --card-bg: #1e293b;
            --accent: #eab308;
            --accent-hover: #ca8a04;
            --text: #f8fafc;
            --danger: #ef4444;
            --success: #22c55e;
            --highlight: #3b82f6;
        }

        * { margin: 0; padding: 0; box-sizing: border-box; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; }

        body {
            background-color: var(--bg-color);
            color: var(--text);
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            overflow-x: hidden;
        }

        /* --- ШАПКА --- */
        header {
            background-color: var(--card-bg);
            padding: 15px 20px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-bottom: 2px solid #334155;
            position: relative;
            z-index: 10;
        }

        .logo { font-size: 1.5rem; font-weight: 800; color: var(--accent); cursor: pointer; }
        
        .header-right { display: flex; align-items: center; gap: 15px; }
        
        .balance-display {
            background: #0f172a;
            padding: 8px 15px;
            border-radius: 20px;
            font-weight: bold;
            border: 1px solid var(--accent);
            color: var(--accent);
            transition: transform 0.2s;
        }

        .balance-display.bump { transform: scale(1.1); }

        .menu-btn { background: none; border: none; color: var(--text); font-size: 1.5rem; cursor: pointer; padding: 5px; }

        /* --- МЕНЮ --- */
        .dropdown-menu {
            position: absolute; top: 100%; right: 20px;
            background-color: var(--card-bg); border: 1px solid #334155;
            border-radius: 8px; width: 220px; display: none;
            flex-direction: column; box-shadow: 0 10px 25px rgba(0,0,0,0.5); overflow: hidden;
        }
        .dropdown-menu.active { display: flex; }
        .dropdown-menu button {
            background: none; border: none; color: var(--text);
            padding: 15px 20px; text-align: left; font-size: 1rem;
            cursor: pointer; border-bottom: 1px solid #334155;
        }
        .dropdown-menu button:hover { background-color: #334155; }
        .dropdown-menu button.logout { color: var(--danger); border-bottom: none; }

        /* --- ЛОББИ --- */
        main { flex-grow: 1; padding: 30px 20px; display: flex; flex-direction: column; align-items: center; }
        h2 { margin-bottom: 20px; font-size: 1.8rem; text-align: center; }
        .games-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 20px; width: 100%; max-width: 900px; }
        .game-card {
            background-color: var(--card-bg); border-radius: 12px; padding: 30px 20px;
            text-align: center; cursor: pointer; transition: 0.2s; border: 2px solid transparent;
        }
        .game-card:hover { transform: translateY(-5px); border-color: var(--accent); }
        .game-card h3 { font-size: 1.4rem; margin-bottom: 10px; }

        /* --- ИГРА --- */
        #game-screen { display: none; flex-direction: column; width: 100%; max-width: 900px; margin: 0 auto; }
        .game-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; background: var(--card-bg); padding: 15px; border-radius: 10px; }
        .btn-back { background: #334155; color: white; border: none; padding: 8px 15px; border-radius: 5px; cursor: pointer; }

        .table {
            background: #064e3b; border: 10px solid #78350f;
            border-radius: 20px; padding: 20px; min-height: 450px;
            display: flex; flex-direction: column; justify-content: space-between;
            box-shadow: inset 0 0 50px rgba(0,0,0,0.5);
        }

        .hand-container { margin-bottom: 20px; padding: 10px; border-radius: 10px; border: 2px solid transparent; transition: 0.3s; }
        .hand-container.active-hand { border-color: var(--accent); background: rgba(234, 179, 8, 0.1); box-shadow: 0 0 15px rgba(234,179,8,0.3); }
        
        .hand { display: flex; gap: 10px; justify-content: center; min-height: 120px; align-items: center; flex-wrap: wrap; }
        .card {
            width: 80px; height: 120px; background: white; border-radius: 8px;
            display: flex; justify-content: center; align-items: center;
            font-size: 2rem; font-weight: bold; box-shadow: 0 4px 8px rgba(0,0,0,0.3);
            animation: dealCard 0.3s ease-out;
        }
        @keyframes dealCard { from { transform: translateY(-50px); opacity: 0; } to { transform: translateY(0); opacity: 1; } }
        .card.red { color: #ef4444; }
        .card.black { color: #0f172a; }
        .card.hidden { background: repeating-linear-gradient(45deg, #1e3a8a, #1e3a8a 10px, #172554 10px, #172554 20px); color: transparent; border: 2px solid white; }

        .score { font-size: 1.2rem; font-weight: bold; text-align: center; margin-bottom: 5px; text-shadow: 1px 1px 2px black; }
        .controls { text-align: center; margin-top: 20px; background: var(--card-bg); padding: 20px; border-radius: 10px; }
        .bet-input { padding: 10px; font-size: 1.2rem; width: 200px; border-radius: 5px; border: 1px solid #475569; background: #0f172a; color: white; margin-bottom: 15px; text-align: center; }
        .action-btns { display: flex; gap: 10px; justify-content: center; flex-wrap: wrap; }
        
        .btn { padding: 10px 20px; font-size: 1rem; font-weight: bold; border: none; border-radius: 8px; cursor: pointer; transition: 0.2s; }
        .btn-primary { background: var(--accent); color: #000; }
        .btn-primary:hover { background: var(--accent-hover); }
        .btn-success { background: var(--success); color: white; }
        .btn-danger { background: var(--danger); color: white; }
        .btn-info { background: var(--highlight); color: white; }
        .btn:disabled { opacity: 0.3; cursor: not-allowed; }
        
        .message { font-size: 1.4rem; font-weight: bold; text-align: center; margin: 15px 0; text-shadow: 2px 2px 4px black; min-height: 35px; color: var(--accent); }
    </style>
</head>
<body>

    <header>
        <div class="logo" onclick="location.reload()">♠️ Пермское Казино</div>
        <div class="header-right">
            <div class="balance-display" id="balance-ui">💰 <span id="balance-val">...</span></div>
            <button class="menu-btn" onclick="toggleMenu()">☰</button>
        </div>
        <div class="dropdown-menu" id="dropdown">
            <button onclick="showLobby()">🎮 Мини-игры</button>
            <button onclick="alert('Настройки в разработке')">⚙️ Настройки</button>
            <button onclick="alert('Пополнение баланса в разработке')">💳 Баланс</button>
            <button class="logout" onclick="resetBalance()">🚪 Сбросить прогресс</button>
        </div>
    </header>

    <main>
        <div id="lobby">
            <h2>Выберите игру</h2>
            <div class="games-grid">
                <div class="game-card" onclick="startBlackjack()">
                    <h3>♠️ Блекджек</h3>
                    <p>Набери 21 очко и обыграй дилера</p>
                </div>
                <div class="game-card" onclick="alert('Покер в разработке')"><h3>♣️ Покер</h3></div>
                <div class="game-card" onclick="alert('Дурак в разработке')"><h3>♥️ Дурак</h3></div>
            </div>
        </div>

        <div id="game-screen">
            <div class="game-header">
                <button class="btn-back" onclick="showLobby()">← В лобби</button>
                <h3>Блекджек</h3>
            </div>

            <div class="table">
                <div>
                    <div class="score">Дилер: <span id="dealer-score">?</span></div>
                    <div class="hand" id="dealer-hand"></div>
                </div>

                <div class="message" id="game-message"></div>

                <div id="player-hands-area"></div>
            </div>

            <div class="controls">
                <div id="bet-controls">
                    <input type="number" class="bet-input" id="bet-amount" placeholder="Ставка" min="5000" value="5000">
                    <br>
                    <button class="btn btn-primary" onclick="dealBlackjack()">Сделать ставку и раздать</button>
                </div>

                <div id="action-controls" style="display: none;">
                    <div class="action-btns">
                        <button class="btn btn-success" id="btn-hit" onclick="hit()">Взять (Hit)</button>
                        <button class="btn btn-primary" id="btn-stand" onclick="stand()">Хватит (Stand)</button>
                        <button class="btn btn-info" id="btn-double" onclick="doubleDown()">Дабл (x2)</button>
                        <button class="btn btn-info" id="btn-split" onclick="split()">Сплит (Split)</button>
                    </div>
                </div>
            </div>
        </div>
    </main>

    <script>
        // --- ЛОГИКА СОХРАНЕНИЯ БАЛАНСА ---
        let balance = 100000; 
        const MIN_BET = 5000;

        // Функция загрузки баланса при старте
        function loadBalance() {
            let savedBalance = localStorage.getItem('casinoBalance');
            if (savedBalance !== null) {
                balance = parseInt(savedBalance);
            } else {
                // Если сохранений нет, ставим 100к и сохраняем
                balance = 100000;
                saveBalance();
            }
            updateBalanceUI();
        }

        // Функция сохранения
        function saveBalance() {
            localStorage.setItem('casinoBalance', balance);
        }

        function updateBalanceUI() {
            document.getElementById('balance-val').innerText = balance.toLocaleString('ru-RU');
            
            // Небольшая анимация при изменении баланса
            const ui = document.getElementById('balance-ui');
            ui.classList.add('bump');
            setTimeout(() => ui.classList.remove('bump'), 200);
        }

        function resetBalance() {
            if(confirm("Вы уверены, что хотите сбросить баланс до 100 000?")) {
                balance = 100000;
                saveBalance();
                updateBalanceUI();
                showLobby();
            }
        }

        // Вызываем загрузку при включении страницы
        window.onload = loadBalance;

        // --- МЕНЮ ---
        function toggleMenu() { document.getElementById('dropdown').classList.toggle('active'); }
        window.onclick = function(event) {
            if (!event.target.matches('.menu-btn')) {
                var dropdowns = document.getElementsByClassName("dropdown-menu");
                for (var i = 0; i < dropdowns.length; i++) {
                    if (dropdowns[i].classList.contains('active')) dropdowns[i].classList.remove('active');
                }
            }
        }

        function showLobby() {
            document.getElementById('lobby').style.display = 'block';
            document.getElementById('game-screen').style.display = 'none';
            document.getElementById('dropdown').classList.remove('active');
        }

        // --- БЛЕКДЖЕК ---
        let deck = [];
        let dealerHand = [];
        let playerHands = []; 
        let activeHandIndex = 0;
        let currentBet = 0;
        let gameActive = false;

        const suits = ['♠', '♣', '♥', '♦'];
        const values = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A'];

        function createDeck() {
            deck = [];
            for (let suit of suits) for (let value of values) deck.push({ suit, value });
            for (let i = deck.length - 1; i > 0; i--) {
                const j = Math.floor(Math.random() * (i + 1));
                [deck[i], deck[j]] = [deck[j], deck[i]];
            }
        }

        function getCardValue(card) {
            if (['J', 'Q', 'K'].includes(card.value)) return 10;
            if (card.value === 'A') return 11;
            return parseInt(card.value);
        }

        function calculateScore(hand) {
            let score = 0, aces = 0;
            for (let card of hand) {
                score += getCardValue(card);
                if (card.value === 'A') aces++;
            }
            while (score > 21 && aces > 0) { score -= 10; aces--; }
            return score;
        }

        function renderCard(card, hidden = false) {
            const div = document.createElement('div');
            div.className = `card ${hidden ? 'hidden' : ''}`;
            if (!hidden) {
                if (card.suit === '♥' || card.suit === '♦') div.classList.add('red');
                else div.classList.add('black');
                div.innerText = card.value + card.suit;
            }
            return div;
        }

        function startBlackjack() {
            document.getElementById('lobby').style.display = 'none';
            document.getElementById('game-screen').style.display = 'block';
            resetUI();
        }

        function resetUI() {
            document.getElementById('dealer-hand').innerHTML = '';
            document.getElementById('player-hands-area').innerHTML = '';
            document.getElementById('game-message').innerText = '';
            document.getElementById('bet-controls').style.display = 'block';
            document.getElementById('action-controls').style.display = 'none';
            document.getElementById('dealer-score').innerText = '?';
            gameActive = false;
        }

        function dealBlackjack() {
            currentBet = parseInt(document.getElementById('bet-amount').value);
            if (isNaN(currentBet) || currentBet < MIN_BET) return alert(`Мин. ставка ${MIN_BET}`);
            if (currentBet > balance) return alert("Мало денег!");

            // Снимаем деньги и СОХРАНЯЕМ
            balance -= currentBet;
            saveBalance(); 
            updateBalanceUI();

            gameActive = true;
            createDeck();
            playerHands = [[deck.pop(), deck.pop()]]; 
            dealerHand = [deck.pop(), deck.pop()];
            activeHandIndex = 0;

            document.getElementById('bet-controls').style.display = 'none';
            document.getElementById('action-controls').style.display = 'block';
            
            renderAllHands();
        }

        function renderAllHands() {
            document.getElementById('dealer-hand').innerHTML = '';
            document.getElementById('dealer-hand').appendChild(renderCard(dealerHand[0]));
            document.getElementById('dealer-hand').appendChild(renderCard(dealerHand[1], gameActive));

            if (!gameActive) {
                document.getElementById('dealer-score').innerText = calculateScore(dealerHand);
            } else {
                document.getElementById('dealer-score').innerText = calculateScore([dealerHand[0]]);
            }

            const area = document.getElementById('player-hands-area');
            area.innerHTML = '';
            
            playerHands.forEach((hand, index) => {
                const container = document.createElement('div');
                container.className = `hand-container ${index === activeHandIndex && gameActive ? 'active-hand' : ''}`;
                
                const scoreDiv = document.createElement('div');
                scoreDiv.className = 'score';
                scoreDiv.innerText = `Рука ${index + 1}: ${calculateScore(hand)}`;
                container.appendChild(scoreDiv);

                const handDiv = document.createElement('div');
                handDiv.className = 'hand';
                hand.forEach(c => handDiv.appendChild(renderCard(c)));
                container.appendChild(handDiv);

                area.appendChild(container);
            });

            updateButtons();
        }

        function updateButtons() {
            if (!gameActive) return;
            const hand = playerHands[activeHandIndex];
            document.getElementById('btn-double').disabled = hand.length !== 2 || balance < currentBet;
            const isPair = hand[0].value === hand[1].value;
            document.getElementById('btn-split').disabled = !(hand.length === 2 && isPair && balance >= currentBet && playerHands.length < 4);
        }

        function hit() {
            if (!gameActive) return;
            playerHands[activeHandIndex].push(deck.pop());
            renderAllHands();

            if (calculateScore(playerHands[activeHandIndex]) > 21) {
                nextHand();
            }
        }

        function stand() {
            if (!gameActive) return;
            nextHand();
        }

        function doubleDown() {
            if (!gameActive) return;
            if (balance < currentBet) return alert("Недостаточно средств!");
            
            balance -= currentBet;
            currentBet *= 2;
            saveBalance(); // СОХРАНЯЕМ
            updateBalanceUI();

            playerHands[activeHandIndex].push(deck.pop());
            renderAllHands();
            setTimeout(nextHand, 500); 
        }

        function split() {
            if (!gameActive) return;
            if (balance < currentBet) return alert("Недостаточно средств!");

            balance -= currentBet;
            saveBalance(); // СОХРАНЯЕМ
            updateBalanceUI();

            const hand = playerHands[activeHandIndex];
            const card1 = hand[0];
            const card2 = hand[1];

            playerHands[activeHandIndex] = [card1, deck.pop()];
            playerHands.splice(activeHandIndex + 1, 0, [card2, deck.pop()]);

            renderAllHands();
        }

        function nextHand() {
            activeHandIndex++;
            if (activeHandIndex < playerHands.length) {
                renderAllHands();
            } else {
                dealerPlay();
            }
        }

        function dealerPlay() {
            gameActive = false; 
            document.getElementById('action-controls').style.display = 'none';
            renderAllHands(); 

            const dealerInterval = setInterval(() => {
                if (calculateScore(dealerHand) < 17) {
                    dealerHand.push(deck.pop());
                    renderAllHands();
                } else {
                    clearInterval(dealerInterval);
                    checkWinners();
                }
            }, 800);
        }

        function checkWinners() {
            const dealerScore = calculateScore(dealerHand);
            let message = "";
            let totalPayout = 0;

            playerHands.forEach((hand, i) => {
                const playerScore = calculateScore(hand);
                
                if (playerScore > 21) {
                    message += `Рука ${i+1}: Перебор. `;
                } else if (dealerScore > 21 || playerScore > dealerScore) {
                    message += `Рука ${i+1}: Победа! `;
                    totalPayout += currentBet * 2;
                } else if (playerScore < dealerScore) {
                    message += `Рука ${i+1}: Проигрыш. `;
                } else {
                    message += `Рука ${i+1}: Ничья. `;
                    totalPayout += currentBet;
                }
            });

            // Начисляем выигрыш и СОХРАНЯЕМ
            balance += totalPayout;
            saveBalance(); 
            updateBalanceUI();
            
            document.getElementById('game-message').innerText = message;
            document.getElementById('bet-controls').style.display = 'block';
        }
    </script>
</body>
</html>